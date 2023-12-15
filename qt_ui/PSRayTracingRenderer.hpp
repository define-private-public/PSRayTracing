#pragma once

#include <QObject>
#include <future>
#include <QTimer>
#include <QElapsedTimer>
#include <QImage>
#include "../render_library/render.h"



/** C++/Qt Object that wraps over the psrt_library's rendering API; so we can use it in QML */
class PSRayTracingRenderer : public QObject
{
public:
    /** status of the renderer */
    enum class Status
    {
        Invalid = 0,        ///< Invalid state
        Idle,               ///< Not doing anything
        InProgress,         ///< Performing a render
        StoppingRender,     ///< When "stop render" request was made, but the render it still waiting a bit before restarting
        Complete,           ///< Completed a render
    };
    Q_ENUM(Status)

private:
    Q_OBJECT
    Q_PROPERTY(QStringList all_scene_ids READ available_scene_ids)
    Q_PROPERTY(qreal render_progress
        READ render_progress
        NOTIFY render_progress_changed
    )
    Q_PROPERTY(QString render_progress_str
        READ render_progress_str
        NOTIFY render_progress_changed
    )
    Q_PROPERTY(QString render_time_str
        READ render_time_str
        NOTIFY render_time_str_changed
    )
    Q_PROPERTY(QString render_file_path READ render_file_path)
    Q_PROPERTY(QSize   render_size      READ render_size)
    Q_PROPERTY(Status status
        READ status
        NOTIFY status_changed
    )
    Q_PROPERTY(QSize screen_resolution
        READ screen_resolution
        NOTIFY screen_geometry_changed
    )

    // Data
    std::future<psrt::Render> _render_result;
    QString _render_file_path = "";
    QSize _render_size;
    QTimer _monitoring_timer;
    QElapsedTimer _stopwatch;
    qreal _render_progress = -1.0;
    QString _render_time_str = "0s";
    Status _status = Status::Idle;

    QSize _screen_resolution;               ///< The "true" screen size of (in physical REAL pixels) of the main display

public:
    explicit PSRayTracingRenderer(QObject *parent = nullptr);
    ~PSRayTracingRenderer();

    Q_INVOKABLE QStringList available_scene_ids() const;        ///< Returns a list (of strings) for all of the Scene id's that can be rendered
    Q_INVOKABLE int num_concurrent_threads_supported() const;   ///< The maximum number of concurrent threads that are supported

private:
    void _set_render_progress(const qreal progress);
    void _set_status(const Status status);
    void _save_result_to_disk();                                ///< Saves the result of the render job to disk (if previous render was successful); Returns where it was saved to, empty if no render ready/

signals:
    void render_in_progress_changed();
    void render_progress_changed();
    void render_time_str_changed();
    void status_changed();
    void screen_geometry_changed();

public slots:
    /** Start a render */
    void start_render(
        const QSize &render_size,
        const QString &scene_id,
        const int samples_per_pixel,
        const int max_depth,
        const int num_threads,
        const QString &seed_str,
        const bool deep_copy_per_thread
    );

    /** Stops an active render */
    void stop_active_render();

    /** Checks if a render is already in progress */
    bool in_progress() const;

    /** Retrive the progress of a render [0.0, 1.0], or -1 if not started, 1.0 if done */
    qreal render_progress() const
        { return _render_progress; }

    /** Get the render progress as a human friendly string */
    QString render_progress_str() const;

    /** Get a human readable string of the time it's been taking to render. */
    QString render_time_str() const
        { return _render_time_str; }

    /**
     * The path to where the render was saved (on disk) and can be read from.
     *
     * Possibly could be emtpy if there is no render, or the render failed
     */
    QString render_file_path() const
        { return _render_file_path; }

    /** Retrive the size of the saved render.  Could be invalid if the render failed or there is none */
    QSize render_size() const
        { return _render_size; }

    /** Retrive the status of the renderer. */
    Status status() const
        { return _status; }

    /** The true resolution of the main screen of the device this is being run on */
    QSize screen_resolution() const
        { return _screen_resolution; }


private slots:
    /**
     * Changes the screen resolution.  This is typically called when the device
     * orientation is changed.
     */
    void _set_screen_resolution(const QSize dim);

    void _on_progress_timer_timeout();                          ///< Will update the status of the render progress and whatnot
    void _on_screen_geometry_changed(const QRect &geometry);    ///< Will update the screen resolution when the device is rotated

};
