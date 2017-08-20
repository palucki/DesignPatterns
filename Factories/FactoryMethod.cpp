#include <iostream>
#include <memory>
#include "doctest.h"

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


//Factory Method to pojedyncza metoda, tworząca ten sam typ obiektu. Tworzenie obiektów jednak może być oddelegowane do klas pochodnych. W praktyce, często tworzymy normalny obiekt biznesowy, w którym występujące po prostu metoda Create. Rzadko tworzy się specjalnie osobną klasę “Factory”, aby zaimplementować factory method.
//Abstract Factory jest odpowiedzialny za tworzenie obiektów należących do tej samej grupy, rodziny. Zwykle tworzy się osobne klasy, aby zaimplementować ten wzorzec. Warto wspomnieć, że różne frameworki IoC wspierają fabryki i automatycznie generują je w formie delegaty Func<T1,T2,…>
