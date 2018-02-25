QT += widgets testlib

SOURCES += minerunittests.cpp \
           ../miner.cpp \
    ../connectionmanager.cpp

HEADERS  += ../miner.h \
    ../connectionmanager.h

LIBS += -L"C:/curl/compiled/curl-7.58.0/AMD64" -llibcurl

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target
