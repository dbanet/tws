<?php
	require_once('config.php');
	$empty = ( empty($_POST['tag']) || empty($_POST['address']) );
	$space = ( substr_count($_POST['tag'], ' ') || substr_count($_POST['address'], ' ') );	
	if( isset($_POST['insert']) and ( !$empty && !$space) ) :
		mysql_query('INSERT INTO clantagtoadressmapping ( tag, address, tooltip, insertTime ) VALUES ( "'.$_POST['tag'].'", "'.$_POST['address'].'", "'.$_POST['tooltip'].'", "'.time().'" );');
	endif;
	if( isset($_GET['delete'] ) ) :
		mysql_query('DELETE FROM clantagtoadressmapping WHERE id = '.$_GET['delete'].';');
	endif;
?>
<html>
	<head>
		<title>The Secret Database of Worms Clans</title>
	</head>
	<body>
		<p><a target="_blank" href="clanlist.txt.php">Clanlist (txt)</a></p>
        <form action="admin.php" method="post" name="clantagtoadressmapping">
		<?php include'textlinesForInclude.php'; ?>
        </form>
		<?php
			$abfrage = mysql_query('SELECT * FROM clantagtoadressmapping') or die(mysql_error());
		?>
		<table cellpadding="5" cellspacing="0" border="1">
			<tr>
				<th>#</th>
				<th>Tag</th>
				<th>Address</th>
				<th>Tooltip</th>
				<th>Date</th>
				<th><img src="img/delete.png" alt="delete" border="0" /></th>
			</tr>
			<?php while($eintrag = mysql_fetch_array($abfrage)) : ?>
			<tr>
				<td><?php echo ++$i; ?></td>
				<td><?php echo $eintrag['tag']; ?></td>
				<td><a target="_blank" title="<?php echo $eintrag['tooltip']; ?>" href="<?php echo $eintrag['address']; ?>">
                <?php echo $eintrag['address']; ?></a></td>
				<td><?php echo $eintrag['tooltip']; ?></td>
				<td><?php echo strftime( '%d. %B %Y', $eintrag['insertTime']); ?></td>	
				<td><a title="delete" href="admin.php?delete=<?php echo $eintrag['id']; ?>">
                <img src="img/delete.png" alt="delete" border="0" /></a></td>
			</tr>
			<?php endwhile; ?>
		</table>
	</body>
</html>
