<?php

	header('Content-type: text/plain; charset=utf-8');
	$file = @file_get_contents('clannames.txt');

	echo $file;

?>