<?php
require_once('api_constants.php');
if ($_POST['apikey'] ===  $apikey) {
        if($_POST['action'] == 'create') {
                $user = $_POST['usr'];
                //Check if new entry matches UID;Name pattern
                if(preg_match('/[a-f0-9]{8};.{3,40}/', $user)) {
                        //Check if user already exists
                        $file = file_get_contents($filename);
                        if (strpos($file, explode(";", $user)[0]) !== false) {
                                die("Error: User is already in file.");
                        }
                        //Write new entry at bottom of the file
                        file_put_contents($filename, $user . "\n", FILE_APPEND);
                        echo "Created " .  $user;
                }
                else {
                        echo "Error: Wrong USR pattern.";
                }
        }
        elseif($_POST['action'] == 'delete') {
                $user = $_POST['usr'] . "\n";
                 //Check if entry to delete matches UID;Name pattern
                if(preg_match('/[a-f0-9]{8};.{3,40}/', $user)) {
                        //Read file
                        $file = file($filename);
                        $pos = array_search($user, $file);
                        
                        if ($pos === false) {
                                die("Error: User not found.");
                        }
                        unset($file[$pos]);
                        $file = implode($file);
                        //Write new file
                        file_put_contents($filename, $file);
                        echo "Deleted " .  $user;
                }
                else {
                        echo "Error: Wrong USR pattern.";
                }
        }
        elseif($_POST['action'] == 'getall') {
                //Retruns all UID and name
                //One user by line
                //First line contains the number of total user
                $file = file($filename);
                $len =  count($file);
                echo $len . "\n";
                for ($i = 0; $i < $len; $i++) {
                        echo $file[$i];
                }
        }
        else {
                //Retruns all UID
                //One user by line
                //First line contains the number of total user
                $file = file($filename);
                $len =  count($file);
                echo $len . "\n";
                for ($i = 0; $i < $len; $i++) {
                        echo explode(";", $file[$i] )[0];
                        echo "\n";
                }
        }
}
else {
        echo "Error: Wrong API Key";
}

/*
user.csv must be in this Format
Hex of UID in lowercase;name
Example:

12bb4f00;MaxMuster
bb420975;ErikaMuster
89e370ae;JaneDoe
*/
?>