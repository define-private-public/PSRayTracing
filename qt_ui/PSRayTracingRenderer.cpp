#include "PSRayTracingRenderer.h"
#include <QStandardPaths>
#include <QDir>
#include <QGuiApplication>
#include <QScreen>
#include "Utils.h"
//#include "../render_library/render.h"
using namespace std;



PSRayTracingRenderer::PSRayTracingRenderer(QObject *parent) :
    QObject(parent)
{
    connect(&_monitoring_timer, &QTimer::timeout, this, &PSRayTracingRenderer::_on_progress_timer_timeout);
    _set_screen_resolution(utils::compute_true_screen_resolution());

    // If/when the device rotations, we need to update the screen's resolution
    connect(QGuiApplication::primaryScreen(), &QScreen::geometryChanged, this, &PSRayTracingRenderer::_on_screen_geometry_changed);
}


PSRayTracingRenderer::~PSRayTracingRenderer() = default;


void PSRayTracingRenderer::start_render(
    const QSize &render_size,
    const QString &scene_id,
    const int samples_per_pixel,
    const int max_depth,
    const int num_threads,
    const QString &seed_str,
    const bool deep_copy_per_thread
) {
    // Guard if we're already doing a render
    if (in_progress())
        return;

    // We're starting one
    _set_status(Status::InProgress);
    _set_render_progress(0.0);
    _render_size = QSize();
    _render_file_path = "";

    psrt::RenderJob job;
    job.render_width = render_size.width();
    job.render_height = render_size.height();
    job.scene_id = scene_id.toStdString();
    job.samples_per_pixel = samples_per_pixel;
    job.max_depth = max_depth;
    job.num_threads = num_threads;
    job.seed_str = seed_str.toStdString();
    job.deep_copy_per_thread = deep_copy_per_thread;

    // Kick off the render
    _render_result = psrt::do_render(job);

    // Start the update timer
    _monitoring_timer.start(50);
    _stopwatch.restart();
}

void PSRayTracingRenderer::stop_active_render()
{
    // Make sure a render is happening
    if (!in_progress())
        return;

    // Set that we're in the process of stopping things
    _set_status(Status::StoppingRender);

    // Tell the library to stop
    psrt::stop_active_render();

    // Reset the some state
    _render_file_path = "";
    _render_size = QSize();
    _set_render_progress(-1.0);
}


bool PSRayTracingRenderer::in_progress() const
    { return psrt::render_in_progress(); }


QString PSRayTracingRenderer::render_progress_str() const
{
    const qreal percent = render_progress() * 100;
    const QString percent_str = QString::number(percent, 'f', 2);
    return QString("%1").arg(percent_str);
}


QStringList PSRayTracingRenderer::available_scene_ids() const
{
    // Create a Qt compatible string list
    QStringList ids;
    for (const std::string &scene_id : psrt::all_scene_ids())
        ids.append(QString::fromStdString(scene_id));

    return ids;
}


int PSRayTracingRenderer::num_concurrent_threads_supported() const
    { return psrt::num_concurrent_threads_supported(); }


void PSRayTracingRenderer::_set_render_progress(const qreal progress)
{
    // Only change if different
    if (qFuzzyCompare(progress, render_progress()))
        return;

    // Update and notify
    _render_progress = progress;
    emit render_progress_changed();
}


void PSRayTracingRenderer::_set_status(const Status status)
{
    // Only change if different
    if (_status == status)
        return;

    // Update and notify
    _status = status;
    emit status_changed();
}


void PSRayTracingRenderer::_save_result_to_disk()
{
    // If we haven't finished a render, return a null image
    if (render_progress() < 1.0)
        return;

    // We have, retrive the data
    _render_result.wait();
    psrt::Render render = _render_result.get();

    // Check render is good
    if (!render.was_succesful())
        return;

    // Convert to a QImage
    QImage img(
        render.image_data.data(),
        render.render_width,
        render.render_height,
        QImage::Format_RGBA8888
    );

    // First try to make the directory
    const QString dest_dir_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QDir dest_dir(dest_dir_path);
    dest_dir.mkpath(".");

    const QString dest_filepath = dest_dir_path + "/render.png";
    img.save(dest_filepath);

    // Finalize and tell QML we're ready to show
    _render_file_path = dest_filepath;
    _render_size = img.size();

    _set_status(Status::Complete);
}


void PSRayTracingRenderer::_set_screen_resolution(const QSize dim)
{
    if (screen_resolution() != dim)
    {
        _screen_resolution = dim;
        emit screen_geometry_changed();
    }
}


void PSRayTracingRenderer::_on_progress_timer_timeout()
{
    if (status() == Status::InProgress)
    {
        const auto total_seconds = static_cast<int>(_stopwatch.elapsed() / 1000);
        const auto percent_complete = static_cast<qreal>(psrt::render_progress());

        // NOTE: It' would be better to use `wait_for` with 0 seconds on the `_render_reuslt` to check if we're done or not, instead of checking the percentage
        const bool render_complete = (percent_complete >= 1.0);

        // Set the string for the render time (don't think I need hours)
        const int secs = total_seconds % 60;
        const int mins = total_seconds / 60;

        // Only show minutes if we have one
        if (mins > 0)
            _render_time_str = QString("%1m %2s").arg(mins).arg(secs);
        else
            _render_time_str = QString("%1s").arg(secs);

        // Update the GUI
        _set_render_progress(percent_complete);
        emit render_time_str_changed();

        // If we completed a render, setup showing it
        if (render_complete)
        {
            _monitoring_timer.stop();
            _save_result_to_disk();
        }
    }
    else if (status() == Status::StoppingRender)
    {
        // If the user stopped the render manually, we need till wailt until the library has told us it has shutdown
        const bool done = !in_progress();
        if (done)
        {
            _monitoring_timer.stop();
            _set_status(Status::Idle);
        }
    }
}

void PSRayTracingRenderer::_on_screen_geometry_changed([[maybe_unused]] const QRect &geometry)
    { _set_screen_resolution(utils::compute_true_screen_resolution()); }
