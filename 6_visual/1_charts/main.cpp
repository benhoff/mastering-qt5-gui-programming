#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QAudioFormat>
#include <QAudioDecoder>


void get_thing()
{
    QAudioFormat format;
    format.setChannelCount(2);
    format.setCodec("audio/x-raw");
    format.setSampleType(QAudioFormat::UnSignedInt);
    format.setSampleRate(48000);
    format.setSampleSize(16);

    QAudioDecoder *audio_decoder = new QAudioDecoder();
    audio_decoder->setAudioFormat(format);
    // FIXME
    // audio_decoder->setSourceFilename("/my/filename/here.wav");
    // FIXME
    // connect(audio_decoder, &QAudioDecoder::bufferReady, this, buffer_slot);

    // audio_decoder->start();
    QAudioBuffer my_buffer = audio_decoder->read();

    QAudioBuffer result_buffer;

}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
