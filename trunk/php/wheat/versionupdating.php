<?php
    if(!file_exists("DownloadLocation.txt")){
        touch("DownloadLocation.txt");
        $file = fopen('DownloadLocation.txt','w+');
        fputs($file,'officialDownload/firstTheWheatSnooperwin32.exe');
        fputs($file,"\n");
        fputs($file,'2.27');
        fclose($file);
    }
    $location=file("DownloadLocation.txt");
    $wheatexists=false;
    if(isset($_POST['toggle']) && !empty($_POST['version']) && file_exists('TheWheatSnooperwin32.exe')){
        function refresh($filename){
            rename('TheWheatSnooperwin32.exe',$filename);
            $file = fopen('DownloadLocation.txt','w+');
            fputs($file,$filename);
            fputs($file,"\n");
            fputs($file,$_POST['version']);
            fclose($file);
        }
        if(isset($_POST['toggle'])){        
            if(trim($location[0])=='officialDownload/firstTheWheatSnooperwin32.exe'){
                refresh('officialDownload/secondTheWheatSnooperwin32.exe');
            } else{
                refresh('officialDownload/firstTheWheatSnooperwin32.exe');                
            }        
        }
        $wheatexists=true;  
    }
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 3.2//EN">

<html>
    <head>
        <meta name="generator" content=
            "HTML Tidy for Windows (vers 14 February 2006), see www.w3.org">

        <title>Version Handling</title>
    </head>    
    <body>
        <?php if(!$wheatexists && isset($_POST['toggle'])):?>
            The File TheWheatSnooperwin32.exe doesnt exist!
            <?php endif; ?>
        <form action="versionupdating.php" method="post">
            <input type="text" name="version" value="<?php 
                    if(isset($_POST['version'])){echo $_POST['version'];} else { echo $location[1]; }
                ?>"><br><br>
            <input value="Toggle versions" type="submit" name="toggle">        
        </form>
    </body>
</html>
