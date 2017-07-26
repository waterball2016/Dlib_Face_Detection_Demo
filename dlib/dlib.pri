android {
    INCLUDEPATH += "$$PWD/android/include ./"
    LIBS += -L$$PWD/android/lib
    LIBS += -ldlib
}

ios {
    INCLUDEPATH += $$PWD/ios/include
    LIBS += -L$$PWD/ios/lib
    LIBS += -ldlib
}
