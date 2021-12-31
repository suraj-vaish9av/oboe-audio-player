#include <jni.h>
#include <string>
#include "utils/logging.h"
#include "audio/PlayerController.h"
#include <android/asset_manager_jni.h>


extern "C" JNIEXPORT jstring JNICALL
Java_com_oboeaudioplayer_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

char* convertJString(JNIEnv* env, jstring str)
{
    if ( !str ) std::string();

    const jsize len = env->GetStringUTFLength(str);
    const char* strChars = env->GetStringUTFChars(str, (jboolean *)0);

    return const_cast<char *>(strChars);
}

std::unique_ptr<PlayerController> mController;

extern "C"
JNIEXPORT void JNICALL
Java_com_oboeaudioplayer_MainActivity_startPlaying(JNIEnv *env, jobject thiz
        , jobject jAssetManager, jstring file_name) {

    AAssetManager *assetManager = AAssetManager_fromJava(env,jAssetManager);

    mController=std::make_unique<PlayerController>(*assetManager);

    char* trackFileName = convertJString(env,file_name);
    mController->start(trackFileName);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_oboeaudioplayer_MainActivity_stopPlaying(JNIEnv *env, jobject thiz) {
    mController->stop();
}