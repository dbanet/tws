<?php
	header('Content-type: text/plain');
	require_once('config.php');
	$abfrage = mysql_query('SELECT * FROM clantagtoadressmapping');
	
	while($eintrag = mysql_fetch_array($abfrage)) {
		echo $eintrag['tag']." ";
		echo $eintrag['address']." ";
		echo $eintrag['tooltip']."\n";
	}
?>