VideoResumenOpenSource
======================

Brief description from wikipedia:

> Video synopsis (often abbreviated V.S.) is an approach to create a short video summary of a long video. It tracks and analyzes moving objects, and converts video streams into a database of objects and activities. The technology has specific applications in the field of video surveillance where, despite technological advancements and increased growth in the deployment of CCTV (closed circuit television) cameras, viewing and analysis of recorded footage is still a costly and labor-intensive and time-intensive task.


In recent years, the use of video storage systems have increase radically. Generally, this records are used for surveillance tasks and allow monitoring activity in particular areas, track certain situations or analyse information from incidents. These systems store large amounts of data and the greatest part of the records have irrelevant information, generating an unnecessary demand of time and money for its storage and analysis. In this work, the development of an open source library to perform video synopsis is presented. The term `video synopsis' refers to the generation of an automatic summary of a video based on digital image processing techniques. With this tool, it is possible to carry out the background substraction, the detection and segmentation of moving objects in a scene, to later on generate the summary of the video. It presents the moving objects with metadata (order and time of appearance) and can present the activity registered in linear time or merged on a common background. Thereby, the library allows the generation of video synopsis of the activity detected, serving as a support of traditional storage systems and facilitating the visual inspection of long videos.

For more information, please see [this paper](http://ieeexplore.ieee.org/document/7497179/)

The private software that can do this task is [BriefCam](http://briefcam.com/). Own library don't have any affiliation with breifcam, and we attend to solve the problem with our way.

Main Features
-------------
+ Video Segmentation.
+ Object search and indexing.
+ Objects tagging and tracking.
+ ...

Requirements
------------

The core of the library is written in C++ and uses Opencv.


| 		        | Version       | Test  |
| :------------ |:-------------:| :----: |
| OpenCV	    | 2.4.13		| :ballot_box_with_check: Ubuntu 14 & 16 - :ballot_box_with_check: Archlinux |


How to use
----------

Just for now, the library it's very hard coded!! kept in mind that this originally was a weekend project in academic ambit.

**Download**
```bash
$ git clone https://github.com/lerker/OpenSourceVS.git
```

**Compile**
```bash
$ cd OpenSourceVS/
$ g++ -std=c++11 -o OpenSourceVS main.cpp `pkg-config --cflags --libs opencv`
```

**Run**
```bash
./OpenSourceVS video2.avi
```

In this example, *video2.avi* is the input video, the arguments is not accepted by the program (hard coded...) and the video file **MUST** be _video2.avi_


You need assitance?
-------------------

I just made a [Dockerfile](https://docs.docker.com/engine/reference/builder/) to download and setup everything!!

Steps

1. Download the Dockerfile
2. Build the image, docker `$ build -t videoresumen:1.0 .`
3. Run the container, `$ docker run -it --name VideoResumen videoresumen:1.0 /bin/bash`
4. In container promt, `./opensourcevs`


TODOs
-----

Please see TODO list.

Contact
-------
Main manteiner: npcuadra@gmail.com (Ponzoni Nelson) as github user @lerker





