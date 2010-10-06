<?php
	$hostname	= 'localhost';
	$dbname		= 'lookias';
	$username	= 'lookias';
	
	//$db			= new PDO( 'mysql:host='.$hostname.';dbname='.$dbname, $username, $password, $options );
	mysql_connect( $hostname, $username, $password);
	mysql_select_db($dbname);
?>