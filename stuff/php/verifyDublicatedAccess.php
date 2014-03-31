<?php                
    function verifyDublicatedAccess($program){
        global $ipaddress;
        $query = mysql_query('SELECT ip,program from downloads_ip_date where program="'.$program.'" and ip="'.$ipaddress.'";') or die(mysql_error());
        while($eintrag = mysql_fetch_array($query)){                    
            return true; 
        }                   
        return false;
    }      
?>
