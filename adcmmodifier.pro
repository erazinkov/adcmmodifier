QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
        dataminer.cpp \
        filewatcher.cpp \
        main.cpp \
        mapperparser.cpp \
        myprocess.cpp \
        newdata.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    adcm_df.h \
    dataminer.h \
    filewatcher.h \
    mapperparser.h \
    mapperquery.h \
    myprocess.h \
    newdata.h
