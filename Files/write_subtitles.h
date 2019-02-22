#include<iostream>
#include<stdlib.h>
#include<string>
#include<string.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>

using namespace std;

string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

int writesubs(string video_file_name,string out_file_name)
{
    string str3= "rm -r -f temp/outaudio.wav";
    if(system(str3.c_str())) return 0;
    //if(setenv("GOOGLE_APPLICATION_CREDENTIALS","/home/stylo/Downloads/key.json",1)) return 0; // set the environmentv variable GOOGLE_APPLICATION_CREDENTIALS change the path to the path of your key
	string str="ffmpeg -i ";
    str=str + video_file_name +" -ar 16000 -ac 1 temp/outaudio.wav";  //output audio with 40000 sampling rate and single channel
cout<<endl<<endl << str<<endl<<endl;
    if(system(str.c_str())) return 0;			//calling the ffmpeg command
    //system("y");
    string str4="ffprobe -i ";
    str4=str4+video_file_name;
    str4=str4 + " -show_entries format=duration -v quiet -of csv=\"p=0\"";
    string duration = exec(str4.c_str());
    int dur=stoi(duration);
    duration= to_string(dur);
    string str2="python script2.py " ;
	str2=str2 + out_file_name;
	str2=str2+" ";
	str2 = str2+duration;				

    if(system(str2.c_str())) return 0;						//calling the python script using system command

	if(system(str3.c_str())) return 0;
	return 1;
	
} 
	//if command runs system() returns 0 else something else
