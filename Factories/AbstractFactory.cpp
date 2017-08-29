#include <iostream>
#include <memory>
#include "doctest.h"


namespace RawDesign
{

#define D_PLATFORM

class Socket
{
public:
    Socket()
    {
#ifdef D_PLATFORM
        //initialize for D_PLATFORM
#else
        //initialize for G_PLATFORM
#endif
    }
};

class FrameProcessing
{
public:
    FrameProcessing()
    {
#ifdef D_PLATFORM
        //initialize for D_PLATFORM
#else
        //initialize for G_PLATFORM
#endif
    }
};

class Router
{
public:
    void handleFrame()
    {
        std::cout << "Received frame in Router \n";
        Socket socket;

        std::cout << "Processing frame in Router\n";
        FrameProcessing frameProcessing;
    }
};

TEST_CASE("Let's create Routers on different platforms...")// * doctest::skip())
{
    Router router;
    router.handleFrame();
}

}

namespace AbstractFactory
{

class Socket
{
public:
    virtual ~Socket() = default;
};

class FrameProcessing
{
public:
    virtual ~FrameProcessing() = default;
};

class PlatformFactory
{
public:
    virtual ~PlatformFactory() = default;
    virtual Socket* createSocket() = 0;
    virtual FrameProcessing* createFrameProcessing() = 0;
};

class Router
{
public:
    Router(PlatformFactory* concreteFactory) : factory(concreteFactory) {}
    void handleFrame()
    {
        std::cout << "Received frame in Router\n";
        auto socket = std::unique_ptr<Socket>(factory->createSocket());

        std::cout << "Processing frame in Router\n";
        auto frameProcessing = std::unique_ptr<FrameProcessing>(factory->createFrameProcessing());
    }
private:
    std::shared_ptr<PlatformFactory> factory;
};

// G Platform specific

class GSocket : public Socket
{
public:
    GSocket()
    {
        std::cout << "Creating GSocket\n";
    }
};

class GFrameProcessing : public FrameProcessing
{
public:
    GFrameProcessing()
    {
        std::cout << "Creating GFrameProcessing\n";
    }
};

class GPlatformFactory : public PlatformFactory
{
public:
    Socket* createSocket()
    {
        return new GSocket;
    }
    FrameProcessing* createFrameProcessing()
    {
        return new GFrameProcessing;
    }
};

class DSocket : public Socket
{
public:
    DSocket()
    {
        std::cout << "Creating DSocket\n";
    }
};

class DFrameProcessing : public FrameProcessing
{
public:
    DFrameProcessing()
    {
        std::cout << "Creating DFrameProcessing\n";
    }
};

class DPlatformFactory : public PlatformFactory
{
public:
    Socket* createSocket()
    {
        return new DSocket;
    }
    FrameProcessing* createFrameProcessing()
    {
        return new DFrameProcessing;
    }
};

TEST_CASE("We'd better use Abstract Factory for this purpose" * doctest::skip())
{
    Router router(new GPlatformFactory);
    router.handleFrame();

    router = Router(new DPlatformFactory);
    router.handleFrame();
}

}
