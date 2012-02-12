SOURCES += \
    main.cpp \
    morsegenerator.cpp \
    morsewavesettings.cpp \
    morseconfigurationmodel.cpp \
    morsegenerator.inl \
    morseplayer.cpp

HEADERS += \
    morsewave.h \
    morsegenerator.h \
    morsewavesettings.h \
    morseconfigurationmodel.h \
    morseconfiguration.h \
    morseplayer.h


win32: LIBS += -L$$PWD/libsndfile/lib/ -llibsndfile-1

win32: INCLUDEPATH += $$PWD/libsndfile/include
win32: DEPENDPATH += $$PWD/libsndfile/include

QT += phonon

unix:!macx:!symbian: LIBS += -lsndfile
