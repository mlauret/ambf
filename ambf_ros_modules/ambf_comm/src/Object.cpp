//==============================================================================
/*
    Software License Agreement (BSD License)
    Copyright (c) 2019, AMBF
    (www.aimlab.wpi.edu)

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution.

    * Neither the name of authors nor the names of its contributors may
    be used to endorse or promote products derived from this software
    without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    \author    <http://www.aimlab.wpi.edu>
    \author    <amunawar@wpi.edu>
    \author    Adnan Munawar
    \version   $
*/
//==============================================================================

#include "ambf_comm/Object.h"
namespace ambf_comm{

Object::Object(std::string a_name, std::string a_namespace, int a_freq_min, int a_freq_max): ObjectRosCom(a_name, a_namespace, a_freq_min, a_freq_max){
  m_objectCommand.enable_position_controller = false;
}

void Object::cur_position(double px, double py, double pz){
    m_trans.setOrigin(tf::Vector3(px, py, pz));
    m_State.pose.position.x = px;
    m_State.pose.position.y = py;
    m_State.pose.position.z = pz;
}

void Object::cur_orientation(double roll, double pitch, double yaw){
    tf::Quaternion rot_quat;
    rot_quat.setRPY(roll, pitch, yaw);
    m_trans.setRotation(rot_quat);
    tf::quaternionTFToMsg(rot_quat, m_State.pose.orientation);
}

void Object::cur_orientation(double qx, double qy, double qz, double qw){
    tf::Quaternion rot_quat(qx, qy, qz, qw);
    m_trans.setRotation(rot_quat);
    tf::quaternionTFToMsg(rot_quat, m_State.pose.orientation);
}

void Object::cur_force(double fx, double fy, double fz){
    tf::Vector3 f(fx, fy, fz);
    tf::vector3TFToMsg(f, m_State.wrench.force);
}

void Object::cur_torque(double nx, double ny, double nz){
    tf::Vector3 n(nx, ny, nz);
    tf::vector3TFToMsg(n, m_State.wrench.torque);
}

void Object::update_af_cmd(){
    m_objectCommand.update(&m_Cmd);
}

void Object::set_wall_time(double a_sec){
    m_State.wall_time = a_sec;
    increment_sim_step();
    m_State.header.stamp = ros::Time::now();
}

void Object::set_userdata(float a_data){
    if (m_State.userdata.size() != 1){
        m_State.userdata.resize(1);
    }
    m_State.userdata[0] = a_data;
}

void Object::set_userdata(std::vector<float> &a_data){
    if (m_State.userdata.size() != a_data.size()){
        m_State.userdata.resize(a_data.size());
    }
    m_State.userdata = a_data;
}

void Object::set_children_names(std::vector<std::string> children_names){
    m_State.children_names = children_names;
}

void Object::set_joint_names(std::vector<std::string> joint_names){
    m_State.joint_names = joint_names;
}

void Object::set_joint_positions(std::vector<float> joint_positions){
    if (m_State.joint_positions.size() != joint_positions.size()){
        m_State.joint_positions.resize(joint_positions.size());
    }
    m_State.joint_positions = joint_positions;
}

extern "C"{

Object* create_object(std::string a_name, std::string a_namespace="/ambf_comm/", int a_min_freq=50, int a_max_freq=1000){
    return new Object(a_name, a_namespace, a_min_freq, a_max_freq);
}

void destroy_object(Object* obj){
    delete obj;
}

}
}
