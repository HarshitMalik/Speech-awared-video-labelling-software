# Speech-awared-video-labelling-software

*This software is developed by [Harshit Malik](harshit.malik999@gmail.com) and [Yogesh Chhabra](chhabra99yogesh@gmail.com ) under guidance of professor [Dr. Abhinav Dhall](http://www.iitrpr.ac.in/cse/abhinavdhall).*

This is sotware developed using **Qt** that uses **FFMPEG** and **CMU Sphinx(speech to text api)** to label a video according to its audio. This software extracts audio from video using ffmpeg and then this audio is converted to text using api. This text is saved in srt format and then video is labelled accordingly with duration of any word being spoken in the video. You can search for any word in the video and can directly jump to that part of the video and you can even save those frames. User interface (UI) of this software is developed using qT creator software.

**Working in brief**

write_subtitles.h, this header file has the function writesubs(const char pointer to video file path,constant char pointer to output txt file path) which calls the script which uses google cloud api for speech to text. The video file is converted to audio using ffmpeg system command and stores the audio with 40000 sampling rate in the resources folder to be used by the python script. Then the script writes the text in subtitles format in the selected output text file.Then our software interface can iteract with the video and the srt file generated and forms a sync between them to make them work together.



**Api Key**

If you need a user key Please contact harshit.malik999@gmail.com or chhabra99yogesh@gmail.com or you can set it up on your own, steps given on google cloud are quite easy to understand
> Link for setting up google cloud environment : https://cloud.google.com/sdk/docs/#deb
```
