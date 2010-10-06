<?php
	$hostname	= 'localhost';
	$dbname		= 'lookias';
	$username	= 'lookias';
	//$password	= 'GQd1XVNZwOCvU3ZYqCT0';
	$password	= '0peration';	
	
	//$db			= new PDO( 'mysql:host='.$hostname.';dbname='.$dbname, $username, $password, $options );
	mysql_connect( $hostname, $username, $password);
	mysql_select_db($dbname);
?>