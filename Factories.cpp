#include <iostream>
#include <memory>
#include "doctest.h"

// LIVE coding session with all factories

namespace RawDesign {


class Ping {
public:
    void send() { std::cout << "Sending Ping frame\n"; }
    void printSize() { std::cout << "Ping frame's size: 1\n"; }
};

class Data {
public:
    void send() { std::cout << "Sending Data frame\n"; }
    void printSize() { std::cout << "Data frame's size: 2\n"; }
};

class RawUdp {
public:
    void send() { std::cout << "Sending RawUdp frame\n"; }
    void printSize() { std::cout << "RawUdp frame's size: 3\n"; }
};

class TrafficGenerator
{
public:
    void sendFrame(std::string type)
    {
        if("Ping" == type)
        {
            Ping frame;
            frame.send();
        }
        else if("Data" == type)
        {
            Data frame;
            frame.send();
        }
        else if("RawUdp" == type)
        {
            RawUdp frame;
            frame.send();
        }
        else
        {
            std::cout << "Unknown frame type\n";
        }
    }
};

class PacketAllocator
{
public:
    void getFrameSize(std::string type)
    {
        if("Ping" == type)
        {
            Ping frame;
            frame.printSize();
        }
        else if("Data" == type)
        {
            Data frame;
            frame.printSize();
        }
        else if("RawUdp" == type)
        {
            RawUdp frame;
            frame.printSize();
        }
        else
        {
            std::cout << "Unknown frame type\n";
        }
    }
};


TEST_CASE("Let's create some frames")// * doctest::skip())
{
    TrafficGenerator generator;
    generator.sendFrame("Ping");

    PacketAllocator allocator;
    allocator.getFrameSize("RawUdp");

    // What is wrong with it?
    // 1) when another class is added the every place similar to sendFrame() must be updated
}

}

namespace SimpleFactory {

class Frame
{
public:
    virtual ~Frame() { }
    virtual void send() = 0;
    virtual void printSize() = 0;
};

class Ping : public Frame
{
public:
    void send() override { std::cout << "Sending Ping frame\n"; }
    void printSize() override { std::cout << "Ping frame's size: 1\n"; }
};

class Data : public Frame
{
public:
    void send() override { std::cout << "Sending Data frame\n"; }
    void printSize() override { std::cout << "Data frame's size: 2\n"; }
};

class RawUdp : public Frame
{
public:
    void send() override { std::cout << "Sending RawUdp frame\n"; }
    void printSize() override { std::cout << "RawUdp frame's size: 3\n"; }
};

class FramesFactory
{
public:
    Frame* createUdpFrame(std::string type)
    {
        if("Ping" == type)
        {
            return new Ping;
        }
        else if("Data" == type)
        {
            return new Data;
        }
        else if("RawUdp" == type)
        {
            return new RawUdp;
        }
        else
        {
            std::cout << "Unknown frame type\n";
            return nullptr;
        }
    }
};

class TrafficGenerator
{
public:
    TrafficGenerator(FramesFactory& factory) : ar_factory(factory) {}
    void sendFrame(std::string type)
    {
        auto frame = std::unique_ptr<Frame>(ar_factory.createUdpFrame(type));
        frame->send();
    }

private:
    FramesFactory& ar_factory;
};

class PacketAllocator
{
public:
    PacketAllocator(FramesFactory& factory) : ar_factory(factory) {}
    void getFrameSize(std::string type)
    {
        auto frame = std::unique_ptr<Frame>(ar_factory.createUdpFrame(type));
        frame->printSize();
    }

private:
    FramesFactory& ar_factory;
};


TEST_CASE("Let's create some frames")// * doctest::skip())
{
    FramesFactory simpleFactory;

    TrafficGenerator generator(simpleFactory);
    generator.sendFrame("Ping");

    PacketAllocator allocator(simpleFactory);
    allocator.getFrameSize("RawUdp");
}
}

namespace FactoryMethod {

class Frame
{
public:
    virtual ~Frame() { }
    virtual void send() = 0;
};

class UdpFrame : public Frame
{
};

class Ping : public UdpFrame
{
public:
    void send() override { std::cout << "Sending Ping frame\n"; }
};

class Data : public UdpFrame
{
public:
    void send() override { std::cout << "Sending Data frame\n"; }
};

class RawUdpFrame : public UdpFrame
{
public:
    void send() override { std::cout << "Sending RawUdp frame\n"; }
};

class TcpFrame : public Frame
{
};

class TcpPing : public TcpFrame
{
public:
    void send() override { std::cout << "Sending TcpPing frame\n"; }
};

class TcpData : public TcpFrame
{
public:
    void send() override { std::cout << "Sending TcpData frame\n"; }
};

class RawTcpFrame : public TcpFrame
{
public:
    void send() override { std::cout << "Sending RawTcp frame\n"; }
};

class TrafficGenerator
{
public:
    virtual ~TrafficGenerator() { }
    virtual Frame* createFrame(std::string type) = 0;
    void sendFrame(std::string type)
    {
        auto frame = createFrame(type);
        if(frame)
        {
            frame->send();
        }
        else
        {
            std::cout << "Unknown frame !!!\n";
        }
    }
};

class UdpTrafficGenerator : public TrafficGenerator
{
public:
    Frame* createFrame(std::string type) override
    {
        if("Ping" == type)
        {
            return new Ping;
        }
        else if("Data" == type)
        {
            return new Data;
        }
        else if("RawFrame" == type)
        {
            return new RawUdpFrame;
        }
        else
        {
            return nullptr;
        }
    }
};

class TcpTrafficGenerator : public TrafficGenerator
{
public:
    TcpFrame* createFrame(std::string type) override
    {
        if("Ping" == type)
        {
            return new TcpPing;
        }
        else if("Data" == type)
        {
            return new TcpData;
        }
        else if("RawFrame" == type)
        {
            return new RawTcpFrame;
        }
        else
        {
            return nullptr;
        }
    }
};


TEST_CASE("Let's create some frames")// * doctest::skip())
{
    auto testTrafficGenerator = std::unique_ptr<TrafficGenerator>(new UdpTrafficGenerator);
    auto releasableTrafficGenerator = std::unique_ptr<TrafficGenerator>(new TcpTrafficGenerator);

    //basic example
    testTrafficGenerator->sendFrame("RawFrame");
    releasableTrafficGenerator->sendFrame("RawFrame");

    //common error handling (as the framework is common)
    testTrafficGenerator->sendFrame("MotoFrame");
    releasableTrafficGenerator->sendFrame("CorruptedFrame");
}
}
