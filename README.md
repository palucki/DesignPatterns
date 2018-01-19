# DesignPatterns
> Simple, real life examples in C++.

The repository contains easy to understand design patterns code examples in C++.
It is a great start for anyone who would like to get familiar with best software engineering practices.

## Getting started
First - clone the repo. Due to some dependencies in examples, you may need to manually install some additional packages. Visit the desired pattern section to get more instructions.


## Patterns described so far:

* [Strategy](https://github.com/palucki/DesignPatterns#strategy)
* [Observer](https://github.com/palucki/DesignPatterns#observer)
* [Decorator](https://github.com/palucki/DesignPatterns#decorator)
* [Factory Method, Abstract Factory](https://github.com/palucki/DesignPatterns#factories)
* [Command](https://github.com/palucki/DesignPatterns#command)

## Strategy

Strategy defines a family of algorithms, encapsulates each one, and makes them interchangeable. Strategy lets the algorithm vary independently from the clients that use it. It captures the abstraction in an interface and buries implementation details in derived classes.

#### Advantages:

* Inheritance binds algorithm with the client code, which makes understanding, conservation and extending more painful (maintenance over faster ‘greenfield’ development) 
* Algorithms (that change, rise & disappear) encapsulated and splitted from the client code
* Removed ‘if’ statements
* Implementation can be set (even dynamically)

#### Building:
```shell
the command
```

## Observer

## Decorator

## Factories

## Command

## Licensing

The code in this project is licensed under MIT license.
