RasPiVJ
=======

Turn the Raspberry Pi into a lightweight VJ plateform.

![screenshot](https://raw.githubusercontent.com/xuv/RasPiVJ/master/screenshot.png)

**Please note that this is in an early stage (alpha), so it lacks functionnality and might require patience to get it working. 
But I'll be happy to reply to your issues and help you in the best I can.**

I've been running this app on a Raspberry Pi B bought in 2013. I would not try to run it on a Raspberry Pi A as it requires some RAM, but you could try to run it on a B+ or Pi2. But for the Pi2, I'm pretty sure the video player will need to be compiled again.

Any input, test or remark from you will help improve this application. 

Installation
------------

- Have a fresh Raspbian system running on your SD card.
- You have to set your RAM split to at least 128MB. You can use `sudo raspi-config` for that. (Iv'e set it up to 256MB)
- Install latest Node.js (I'm using v0.12.0) http://node-arm.herokuapp.com/
- Clone this repository in the /home/pi folder
- Go into raspivj/ folder and do `npm install`. This should install the necessary node modules.

I think that's all you'll need. Please tell me if there is something I'm missing.  
If you'd like to modify the video player (multiPlayer), you will need [openFrameworks](http://openframeworks.cc/). 

Manual
------
All your videos should be in the videos/ folder.  
As for now, you will need to make thumbnails of your videos yourself and have them in the subfolder called thumbs/  
Be sure to name your thumbnails the same way you name your video files. **It's important that all your videos have thumbnails.**  
The app is quite stupid right now. It assumes that each video has a thumbnail and that the first thubmnail in the thumbs/ folder corresponds to the first video in the video/ folder.  
Your videos need to be h264 encoded in a mov container with some sound (even if it's silence) for best results. See a short guide to convert your videos here: http://w.xuv.be/projects/raspivj#how_to_convert_your_videos_fro_the_raspivj
Also, your videos resolution shuold not be higher than 1024, although you might try a 720p. Don't try to go beyond that, it will crash the player. For fast transitions and loading, keep it as low as possible.

The app is started like this:  
`node raspivj/raspivj.js`

There is a raspivj.crontab file that you can use to start the app when the Pi is booting. Set it up by running this:  
`crontab raspivj/raspivj.crontab`

To access the interface, you should know the IP of the Raspberry Pi. Then point a browser to this address:  
http://<ip-of-your-raspberry>:3000

Further notes
-------------
http://w.xuv.be/projects/raspivj

Author
------
Julien Deswaef @xuv

TODO
----
- Make an external config file for setting up the video folder
- Rework the web interface (add icons,…)
- Add shaders

Video examples
--------------
All videos in the video/ folder were downloaded from http://www.beeple-crap.com/resources.php  
"These files are available under a sort of "open source" concept. They are intended for educational use but really can be used for whatever the fuck you want. You don't need to credit me for whatever commercial or non-commercial use you make of them, but if you could shout me a holla with any project that do come from them, I'd love to see it :)"


