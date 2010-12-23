# This utility is for final project of Computer Architecture
# Author: Hua Xu
# EE of Columbia University
# This file can only be modified by my team member.
#============================Configuration==================================================================#
config_file='L2Single.conf'
config_power='L2SinglePower.conf'
Datasets='6 64 4'
#====================Don't modified below this line unless you know what you are doing :) ==============#####
if [ "$1" = "-cal" ]; then
    ./wattchify ../confs/$config_file ../confs/tmp.conf
    ./cactify ../confs/tmp.conf ../confs/$config_power

elif [ "$1" = "-run" ]; then
    ./sesc.smp -c../confs/$config_power -dsimsmall -fresult ../programy/programy.sesc -v $Datasets

elif [ "$1" = "-res" ]; then
    ../scripts/report.pl simsmall.result

elif [ "$1" = "-res_s" ]; then
    echo "Give a proper name for this result!"
    read name
    ../scripts/report.pl simsmall.result > Result/result_$name

elif [ "$1" = "-see" ]; then
    echo "What do you want to see?"
    read object
    grep $object -r -n Result/*

elif [ "$1" = "-auto" ]; then
    echo "Remove files"
    rm ../confs/tmp.conf ../confs/$config_power
    rm *.result
    echo "Running Power Estimation!"
    ./wattchify ../confs/$config_file ../confs/tmp.conf
    ./cactify ../confs/tmp.conf ../confs/$config_power

    echo "Program is running, Please wait........................"
    ./sesc.smp -c../confs/$config_power -dsimsmall -fresult ../programy/programy.sesc $Datasets
    echo "Give a proper name for this result!"
    read name
    ../scripts/report.pl simsmall.result > Result/result_$name
    emacs Result/result_$name
 
elif [ "$1" = "-h" ]; then
    echo "-cal    Calculate Power dissipation."
    echo "-run    Run simulation."
    echo "-res    Generate Result and display."
    echo "-res_s  Save result to file"
    echo "-see    See partion of result."
    echo "-auto   Auto run the whole process!"
else
    echo "Type -h for help!"
fi