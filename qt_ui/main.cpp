#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "UIMathHelper.h"
#include "PSRayTracingRenderer.h"


int main(int argc, char *argv[]) {
    qmlRegisterType<PSRayTracingRenderer>("SixteenBPP", 1, 0, "PSRayTracingRenderer");
    qmlRegisterType<UIMathHelper>("SixteenBPP", 1, 0, "UIMathHelper");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Create the renderer and bind it w/ a global
    auto renderer = new PSRayTracingRenderer(&app);
    engine.rootContext()->setContextProperty("g_renderer", renderer);

    // Create the UI math helper (and expose it to Qml)
    auto ui_math_helper = new UIMathHelper(&app);
    engine.rootContext()->setContextProperty("g_ui_math_helper", ui_math_helper);

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
