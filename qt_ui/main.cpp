#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "UIMathHelper.h"
#include "PSRayTracingRenderer.h"
#include "Settings.h"


int main(int argc, char *argv[])
{
    qmlRegisterType<PSRayTracingRenderer>("SixteenBPP", 1, 0, "PSRayTracingRenderer");
    qmlRegisterType<UIMathHelper>(        "SixteenBPP", 1, 0, "UIMathHelper");
    qmlRegisterType<Settings>(            "SixteenBPP", 1, 0, "Settings");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Setup some globals that communicate between Qml & C++
    auto render_engine = new PSRayTracingRenderer(&app);
    auto settings = new Settings(render_engine->num_concurrent_threads_supported(), &app);
    auto ui_math_helper = new UIMathHelper(&app);
    engine.rootContext()->setContextProperties({
        {"g_renderer",       QVariant::fromValue(render_engine)},
        {"g_settings",       QVariant::fromValue(settings)},
        {"g_ui_math_helper", QVariant::fromValue(ui_math_helper)},
    });

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.addImportPath(":imports/");
    engine.load(url);

    return app.exec();
}
