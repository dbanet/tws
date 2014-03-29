<?php
	$hostname	= 'localhost';
	$dbname		= 'lookias';
	$username	= 'lookias';	
	$password	= '123';
	mysql_connect( $hostname, $username, $password);
	mysql_select_db($dbname);
?>