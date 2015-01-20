<?php 
include "php_serial.class.php";

//Set max execution time infinite
ini_set('max_execution_time', 0);

// Start the session, for save last email data in a session variable and send notification to arduino only if there is new email (new data)
session_start();




//Replace username and password with personal gmail 
$hostname = '{imap.gmail.com:993/imap/ssl}INBOX';
$username = 'personalmail@gmail.com';
$password = 'password';

$mbox = imap_open($hostname, $username, $password) or die('Cannot connect to Gmail: ' . imap_last_error());

$count = imap_num_msg($mbox);

$str = "";
$lastData = "";
$oldData = $_SESSION["oldData"];
$nMails = 0;

//For every email
for($i = $count; $i >= 1; $i--) {
    $header = imap_header($mbox, $i);
    if( $i == $count){
	    
	    $lastData.= $header->date; //Keep last data
	    echo $lastData;
    }
    if($header->Unseen == 'U') { //Keep only unseen emails
	
	$from = $header->from;
   	 foreach ($from as $id => $object) {
        	$fromname = $object->personal;
    	}
	$email = "F: ".$fromname." - S: ".$header->subject." = ";
	if(strlen($str) + strlen($email) <= 320){
		$str.= $email; //Add every email
		
	}
$nMails++;
	
    }
}

imap_close($inbox);


//echo "  ----- " .$str;

//echo "<br>SESSION DATA ".$_SESSION["oldData"]."<br>";
//echo "<br>OLDDATA".$oldData."<br>";
//echo "<br>LASTDATA".$lastData."<br>"; 
//$oldData="2";

//IF THERE IS A NEW EMAIL I SEND NOTIFICATION TO ARDUINO
if($lastData != $oldData ){
	$oldData = $lastData;
	$_SESSION["oldData"] = $oldData;
	echo "CHANGE OLD DATA: ".$oldData;
	
	
	error_reporting(E_ALL);
	ini_set("display_errors", 0);
	//define arduino serial port consant
	define("PORT","/dev/ttyUSB0");
	

	$max = 320;

	$n_parts = (int)(strlen($str) / $max);
	if((strlen($str) % $max) > 0) $n_parts++;
	
	sendToArduino($nMails." - ".$n_parts);
	echo "qui";
	for($i = 0; $i < $n_parts; $i++) {
	
    		if($i != $n_parts - 1)
    			$part = substr($str, $i*$max, $max);
   	else
        	$part = substr($str, $i*$max);
	//echo "<br>PART: ".$part." partlen: ".strlen($part)."<br>";
	//echo "<br> INVIO: ".$part."<br>";
   	sendToArduino($part);
	usleep(2000000); //time interval for each reading by arduino from serial
	}
 
	
   	
}
else
	echo"NOT CHANGE OLD DATA: ".$oldData;




//Function used for connect and send data to arduino
function sendToArduino($toSend){
	echo "<br>TOSEND: ".$toSend." partlen: ".strlen($toSend)."<br>";
    	

    	$serial = new phpSerial;
	$serial->deviceSet(PORT);
	$serial->confBaudRate(9600);
        $serial->confParity("none");
        $serial->confCharacterLength(8);
        $serial->confStopBits(1);
        $serial->confFlowControl("none");
        $serial->deviceOpen();
	usleep(2000000); 
        $serial->sendMessage($toSend);
    	$serial->deviceClose(); 


}


//Refresh script every $sec second for check new emails

$page = $_SERVER['PHP_SELF'];
$sec = "120";
header("Refresh: $sec; url=$page");


?>
