<?php
	require_once('config.php');
	mysql_query('INSERT INTO clantagtoadressmapping ( tag, address, tooltip, insertTime ) VALUES ( "'.$_POST['tag'].'", "'.$_POST['address'].'", "'.$_POST['tooltip'].'", "'.time().'" );');	
	
	$abfrage = mysql_query('SELECT * FROM clantagtoadressmapping');	
?>
<html>
	<head>
	
	</head>
	<body>
		<table cellpadding="5" cellspacing="0" border="1">
			<tr>
				<th>Tag</th>
				<th>Addresse</th>
				<th>Tooltip</th>
			</tr>
			<?php while($eintrag = mysql_fetch_array($abfrage)) : ?>
			<tr>
				<td><?php echo $eintrag['tag']; ?></td>
				<td><?php echo $eintrag['address']; ?></td>
				<td><?php echo $eintrag['tooltip']; ?></td>
			</tr>
			<?php endwhile; ?>
		</table>
		<form action="index.php" method="post">
			<input type="submit" name="zurueck" value="zur&uuml;ck" />
		</form>
	</body>
</html>