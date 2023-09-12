# simple-protocol-chatGPT

<strong>TL/DR</strong> Creating a small protocol with ChatGPT for multiple languages

## introduction

This time I will explore protocol based code generation for multiple languages using ChatGPT, something that I’m really fond of and enjoyed doing in the past.

This time I will use C++, Python & rust. I will also make an attempt to use Zig a language that I'm not familiar at all.

## Coding challenge

I will make a typical protocol with the a 2 byte source, destination field, a ID/counter that increases each time a node sends a packet, a byte index and subindex field and finally a variable data field max 10 bytes. The index and subindex is a common way like OPENCAN or ECSS to use a table and describe the data field.  I will then create different nodes in different languages, each node having a unique id and using a main node to multiple secondary nodes. The nodes will be connected to the main node and it will route the messages accordingly.

The underlying method of transferring the data will be UDP. Each node will connect to the main node using different ports and the main node will route the messages based on the unique ID.Now there are better ways to do this sort of communication, like MQTT etc but this is not meant to be a production code or efficient way to communicate but more of an exploration of the code generating capabilities of AI and ChatGPT.

### Functionality

I’m going to create a small number of nodes that when they receive a message with index 17 and sub index 1, they will respond with a message with index 17 and sub index 2. For now it won’t be any data.

It will be a master node that sends the message every x seconds and verify that the message was received.

This services is used to simulate a heartbeat, a way to make sure that all nodes are up and running.

## During the "development"

This 2nd attempt was more frustrating but the frustration came mostly from  poor planning in what I'm trying to do, I simply I didn't think it through enough. And that no matter how good an AI is, it cannot do much for poor design.

While I tried to make no code changes, sometimes it was much quicker to do some manual changes than to instruct chatGPT to do it for yourself. This come mostly I think by the complexity of having mutliple classes and applications in one chat.

Generating the code for python was a breeze and very fast. I only had to make the changes I made for the C++ classes. Rust was a bit more difficult but I attribute that to my unfamiliarity with the language.

## Takeaways

I found out that after I while I didn't trust the AI to correct small changes while keeping the code it works the code. SO any small changes that was needed I did them manually.

I really like this way of development, it keeps the fun part while remove the boring part of writing repititive/boilerplate code. It also gives me a sense of better communicating with the machine, there is some sort of dialog which makes it metaphusically interesting. I'm not sure that I can go back to the regular way of working, pressing dots and semicolons. It is definetely exciting.

I also tried to use zig, a language that I'm totally unfamiliar but it didn't work and I didn't have the patience to try and make it work. From a quick look it doesn't offer UDP connectivity yet.

In the end, as I said it was fun. Keeping the complexity of the code down, will also make using AI much faster.

## Next steps.

So far I've used only ChatGPT but there are other tools that I would like to explore and compare. Now that I have a bit more understanding I will do a small code example in different AI tools.

## Used versions

| Tool | Version   |
|----------|------|
| Ubuntu | 20.04 LTS |
| AI | ChatGPT 3.5 Aug 3 version |
| Rust | 1.72.0 |
| Python | 3.8.10 |
| C/C++ | gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1) |
