#!/usr/bin/env expect
# scp_beagleplay_expect.sh
# Copies the built binary to the BeaglePlay using password "1212".
# Keep this file private: chmod 700 scp_beagleplay_expect.sh

# ---------- STATIC CONFIG ----------
set user       "vlad"
set host       "192.168.68.70"
set localfile  "/home/vlad/projects/beagleplay/build/project_beagleplay"
set remotepath "/home/vlad/bin"
set password   "1212"
# -----------------------------------

# sanity check local file
if {![file exists $localfile]} {
    puts "ERROR: local file not found: $localfile"
    exit 1
}

# wait as long as needed
set timeout -1

# run scp
spawn scp $localfile $user@$host:$remotepath/

expect {
    -re "Are you sure you want to continue connecting.*" {
        send "yes\r"
        exp_continue
    }
    -re {([Pp]assword|Enter passphrase for key).*:} {
        send "$password\r"
    }
    eof {
        # finished (could be key-based auth, or after password)
    }
}

# wait for scp to finish and get exit status
expect eof
set waitval [wait]
set exitstatus [expr {[lindex $waitval 3]}]

if {$exitstatus == 0} {
    puts "SCP finished successfully."
    exit 0
} else {
    puts "SCP exited with status $exitstatus"
    exit $exitstatus
}
