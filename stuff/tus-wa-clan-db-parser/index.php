<?php

	/*
	 *
	 * Author: Zexorz (http://www.tus-wa.com/profile/alternax) 
	 * Using for: The Wheat Snooper 2.9+ (https://github.com/dbanet/tws)
	 * License: GPLv2
	 * Date: 09/05/2014
	 * Custom Note: Goodbye Yahoo API, i prefered write custom parser :)
	 *
	 */

	header('Content-type: text/plain; charset=utf-8');

	$file = fopen('clannames.txt', 'w');

	$parse_html_document = @file_get_contents('http://www.tus-wa.com/groups');

	$dom = new DOMDocument;
	$dom->LoadXML($parse_html_document);

	$file_to_parse = simplexml_import_dom($dom);

	foreach($file_to_parse->body->div[2]->div[0]->div->div[2]->div->div->table->tbody->tr as $item) { // Huh dat parsing xD
		if($item->td[2]== "Clan") {
			if($item->td[3]->span == "Active") {
				echo $item->td[1]->a." ";

				if($item->td[4]->a->href == NULL) { // If site is NULL
					$clan_site = "http://www.tus-wa.com/groups/".$item->td[1]->a."/"; //Return TUS LINK
				} else { // Othervise
					if($item->td[4]->a['href'] == "http://") {
						$clan_site = "http://www.tus-wa.com/groups/".$item->td[1]->a."/"; //Return TUS LINK
					} else {
						$clan_site = $item->td[4]->a['href']; //Return Clan Site!
					}
				}

				echo $clan_site." ";
				echo $item->td[1]->div->em.PHP_EOL;

				fwrite($file, $item->td[1]->a." ".$clan_site." ".$item->td[1]->div->em.PHP_EOL);

			}
		}
	}

	fclose($file);

?>