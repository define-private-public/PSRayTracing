#pragma once

#include <QObject>
#include <QSize>
#include <QString>
#include <QSettings>


class Settings : public QObject
{
    Q_OBJECT

    // Setting must be exposed as properties so we can talk with QML
    Q_PROPERTY(QSize render_size          READ render_size           WRITE set_render_size)
    Q_PROPERTY(QString scene_id           READ scene_id              WRITE set_scene_id)
    Q_PROPERTY(int samples_per_pixel      READ samples_per_pixel     WRITE set_samples_per_pixel)
    Q_PROPERTY(int max_depth              READ max_depth             WRITE set_max_depth)
    Q_PROPERTY(int num_threads            READ num_threads           WRITE set_num_threads)
    Q_PROPERTY(QString seed_str           READ seed_str              WRITE set_seed_str)
    Q_PROPERTY(bool deep_copy_per_thread  READ deep_copy_per_thread  WRITE set_deep_copy_per_thread)

    // Data
    int _num_concurrent_threads_supported = 1;      // Note that this is device specific
    QSettings _settings_store;                      ///< The underlying object that stores all of the settings


public:
    explicit Settings(const int num_concurrent_threads_supported, QObject *parent = nullptr);
    ~Settings() = default;

private:
    void _set_to_initial_values();                          ///< Sets the intial values for all settings

public:
    Q_INVOKABLE void reset_render_settings_to_default();    ///< Resets just the render settings

    /*== the render settings ==*/
    QSize render_size() const;
    void set_render_size(const QSize &size);

    int samples_per_pixel() const;
    void set_samples_per_pixel(const int num_samples);

    int max_depth() const;
    void set_max_depth(const int depth);

    int num_threads() const;
    void set_num_threads(const int thread_count);

    QString seed_str() const;
    void set_seed_str(const QString &seed);

    bool deep_copy_per_thread() const;
    void set_deep_copy_per_thread(const bool do_deep_copy);

    // Scene Id is not considered a render setting
    QString scene_id() const;
    void set_scene_id(const QString &id);
};
