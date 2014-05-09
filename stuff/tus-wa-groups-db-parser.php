<?php // GNU GPLv2 //
/**
 * @author  ZexorZ
 */

header('Content-type: text/plain; charset=utf-8'); // We need return correctly plain text, and not rendered HTML

$f = simplexml_load_file("https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20html%20where%20url%3D%22http%3A%2F%2Fwww.tus-wa.com%2Fgroups%2F%22%20and%0A%20%20%20%20%20%20xpath%3D'%2F%2Ftable%5B%40class%3D%22table_grid%22%5D%2Ftbody'&diagnostics=true");

foreach($f->results->tbody->tr as $item) {
  if($item->td[2]->p == "Clan") {
    if($item->td[3]->span == "Active") {
      echo $item->td[1]->a." ";
        if($item->td[4]->a->href == NULL) { // If site is NULL
          echo "http://www.tus-wa.com/groups/".$item->td[1]->a."/ "; //Return TUS LINK
        } else { // Othervise
        	if($item->td[4]->a['href'] == "http://") {
        		echo "http://www.tus-wa.com/groups/".$item->td[1]->a."/ "; //Return TUS LINK
        	} else {
        		echo $item->td[4]->a['href']." "; //Return Clan Site!
        	}
        } // ENDIF
      echo $item->td[1]->div->em."".PHP_EOL;
    }
  }
}

?>