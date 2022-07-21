#include "Settings.h"


// Keys for the general section
constexpr char version_key[]              = "settings_version";

// Keys for the render settings
constexpr char render_size_key[]          = "render_settings/size";
constexpr char samples_per_pixel_key[]    = "render_settings/samples_per_pixel";
constexpr char max_depth_key[]            = "render_settings/max_dpeth";
constexpr char num_threads_key[]          = "render_settings/num_threads";
constexpr char seed_str_key[]             = "render_settings/seed_str";
constexpr char deep_copy_per_thread_key[] = "render_settings/deep_copy_per_thread";

constexpr char scene_id_key[]             = "controls_bar/scene_id";

// Current version of the settings system
constexpr int current_settings_system_verison = 1;


Settings::Settings(const int num_concurrent_threads_supported, QObject *parent) :
    QObject(parent),
    _num_concurrent_threads_supported(num_concurrent_threads_supported),
    _settings_store("16BPP.net", "PSRayTracing")
{
    // First make sure some settings file exists (by checking the version), if not, then make them
    const bool exists = _settings_store.contains(version_key);
    if (!exists)
        _set_to_initial_values();
}


void Settings::_set_to_initial_values()
{
    // Setup the intial settings values
    _settings_store.setValue(scene_id_key, "book2::final_scene");
    reset_render_settings_to_default();
}


Q_INVOKABLE void Settings::reset_render_settings_to_default()
{
    const int half_core_count = _num_concurrent_threads_supported / 2;

    _settings_store.setValue(version_key,              current_settings_system_verison);
    _settings_store.setValue(render_size_key,          QSize(960, 540));
    _settings_store.setValue(samples_per_pixel_key,    10);
    _settings_store.setValue(max_depth_key,            50);
    _settings_store.setValue(num_threads_key,          std::max(half_core_count, 1));
    _settings_store.setValue(seed_str_key,             "ASDF");
    _settings_store.setValue(deep_copy_per_thread_key, true);
}


QSize Settings::render_size() const
    { return _settings_store.value(render_size_key).toSize(); }

void Settings::set_render_size(const QSize &size)
    { _settings_store.setValue(render_size_key, size); }


int Settings::samples_per_pixel() const
    { return _settings_store.value(samples_per_pixel_key).toInt(); }

void Settings::set_samples_per_pixel(const int num_samples)
    { _settings_store.setValue(samples_per_pixel_key, num_samples); }


int Settings::max_depth() const
    { return _settings_store.value(max_depth_key).toInt(); }

void Settings::set_max_depth(const int depth)
    { _settings_store.setValue(max_depth_key, depth); }


int Settings::num_threads() const
    { return _settings_store.value(num_threads_key).toInt(); }

void Settings::set_num_threads(const int thread_count)
    { _settings_store.setValue(num_threads_key, thread_count); }


QString Settings::seed_str() const
    { return _settings_store.value(seed_str_key).toString(); }

void Settings::set_seed_str(const QString &seed)
    { _settings_store.setValue(seed_str_key, seed); }


bool Settings::deep_copy_per_thread() const
    { return _settings_store.value(deep_copy_per_thread_key).toBool(); }

void Settings::set_deep_copy_per_thread(const bool do_deep_copy)
    { _settings_store.setValue(deep_copy_per_thread_key, do_deep_copy); }


QString Settings::scene_id() const
    { return _settings_store.value(scene_id_key).toString(); }

void Settings::set_scene_id(const QString &id)
    { _settings_store.setValue(scene_id_key, id); }
