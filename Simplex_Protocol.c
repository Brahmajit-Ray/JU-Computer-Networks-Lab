#include <stdio.h>
#include <stdlib.h>

typedef enum{frame_arrival} event_type;
typedef enum{data} frame_kind;

int seq_no=1;

typedef struct{
	frame_kind kind;
	int seq;
	int ack;
	int info;
}frame;

frame frame_through_channel;

event_type global_event=frame_arrival;

void wait_for_event(event_type event){            //If frames available,then proceed
	while(global_event!=frame_arrival){
	
	}
	event=frame_arrival;
}

int get_data(){
	int d;
	printf("Enter packet data to be sent:");      //Get some data for the frame
	scanf("%d",&d);
	return d;
}

frame make_frame(frame_kind kind,int info,int ack){   //Get data from the user for the frame
	frame f;
	f.kind=kind;
	f.seq=seq_no;
	f.ack=ack;
	f.info=info;
	seq_no++;
	
	return f;	
}

void put_frame_on_channel(frame f){
	frame_through_channel=f;
}

void send_frame(frame f){
	put_frame_on_channel(f);
}


frame receive_frame(){
	frame recv=frame_through_channel;
	return recv;
}


int extract_data(frame f){
	return f.info;
}

void sender(){
	int frame_data;
	frame generated;
	
	frame_data=get_data();
	generated=make_frame(data,frame_data,0);
	send_frame(generated);
}


void receiver(){
	event_type event;
	frame received;
	int extracted_data;
	
	wait_for_event(event);
	received=receive_frame();
	extracted_data=extract_data(received);
	printf("Data extracted from the sent frame:%d\n",extracted_data);
}

int main(){
	int track=0;
	printf("Simplex Method Simulator for upto 5 packets\n");
	while(track<5){
		sender();
		receiver();
		track++;
	}
}


