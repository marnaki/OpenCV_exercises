#!/usr/bin/env python3

import rospy

rom geometry_msgs.msg import Twist
from geometry_msgs.msg import TwistStamped

def callback(data):
    rospy.loginfo(data.data) #controllare cosa prende

def listener():
    rospy.init_node('listener',anonymous=True)

    rospy.Subcriber("cmd_vel", Twist, callback)
    
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()