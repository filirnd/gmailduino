#GMAILDUINO (Gmail notifier with Php and Arduino)

##What is GmailDuino 
GmailDuino is a project created by students of Computer Science Department (University of Catania). It is a Gmail Notifier that used and Arduino UNO (http://www.arduino.cc/) with a LCD Display, conjointly a Buzzer and a RGB Led, for notify when a new email is arrived, and allow to read sender and subject too.
Notifications are sent by php server (index.php) every 120 second (settable).
Good fun!

##Installation and configuration

###PHP Configuration
For execute php script you need a web server installed ( we recommend using Linux machine with Apache web server installed) and php installed inside.
Futhermore you must install imap module for php with following command (on debian & derivate):

	apt-get install php5-imap 


Put index.php file into your personal web server directory ( our default directory is /var/www/html ).
Open index.php and set with your personal information:

- $username = 'personalmail@gmail.com';
- $password = 'personalpassword';


In following line of index.php you must replace "/dev/ttyUSB0" with your personal serial port name, where arduino is plugged:
- define("PORT","/dev/ttyUSB0");

Lastly you add to your user and to server user the serial group (default is "dialout" but you can read this with command "ls -l /dev/ | grep tty*"):

	sudo su
	usermod -g dialout username
	groups username 
	usermod -a -G dialout www-data 
	groups www-data

Note: replace username with your personal user, and www-data with your server user.




###ARDUINO Configuration


###Credits 
Have participated at project (Alphabetical Order):

- Cantarella Danilo (http://cantarelladanilo.com)
- Maccarrone Roberta (http://robertamaccarrone.altervista.org)
- Parasiliti Parracello Cristina (http://cristinaparasiliti.altervista.org)
- Randazzo Filippo (http://randazzofilippo.com);
- Safarally Dario (http://dariosafarally.altervista.org);
- Siragusa Sebastiano (http://sebastianosiragusa.altervista.org/)
- Vindigni Federico ()
