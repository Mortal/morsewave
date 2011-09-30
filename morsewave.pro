SOURCES += \
    main.cpp \
    morsegenerator.cpp \
    morsewavesettings.cpp

HEADERS += \
    morsewave.h \
    morsegenerator.h \
    morsewavesettings.h


win32: LIBS += -L$$PWD/libsndfile/lib/ -llibsndfile-1

INCLUDEPATH += $$PWD/libsndfile/include
DEPENDPATH += $$PWD/libsndfile/include


