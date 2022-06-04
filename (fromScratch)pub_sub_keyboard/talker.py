#!/usr/bin/env python3

import rospy
from std_msg.msg import Float64 #controllare tipo messaggio

def talker():
    pub=rospy.Publisher('chatter',Float64, queue_size=10) #controllare tipo messaggio
    rospy.init_node('talker',anonymous=True)
    rate=rospy.Rate(1) #1Hz
    while not rospy.is_shutdown

        #prendo cose dalla keyboard
        rospy.loginfo() #printa il messaggio sul terminale MA non lo pubblica sul topic
        pub.publish() #pubblica il msg sul /chatter topic
        rate.spleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass