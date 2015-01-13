<!DOCTYPE html>
<html>
<body>

<h1>Search Database</h1>

Use SQL query to search our movie database!

<form action="search_database.php" method="GET">
<textarea name="Query" cols="60" rows="8"></textarea>
<input type="submit" value="Query">
</form>

Note: tables and fields are case sensitive. Run "show tables" to see the list of available tables.<br><br>

<a href="add_actor_director.php">Add a new actor or director</a> <br><br>
<a href="add_movie.php">Add a new movie</a> <br><br>
<a href="add_comments.php">Add a new movie comment</a> <br><br>
<a href="query.php">Return home</a> <br><br>

<table border=1 cellspacing=1 cellpadding=2>
<!--<tr align=center><td><b>id</b></td><td><b>title</b></td><td><b>year</b></td><td><b>rating</b></td><td><b>company</b></td></tr>-->
<?php
	$db_connection = mysql_connect("localhost", "cs143", ""); #opens connection to database
	if(!$db_connection){
		$errmsg = mysql_error($db_connection);
		print "Connection failed: $errmsg <br />";
		exit(1);
	}
	mysql_select_db("CS143", $db_connection); #opens CS143 database

	$query = $_GET["Query"];
	if($_GET["Query"] == NULL){
		exit(0);
	}
	print "Your query: " . "$query";
	print "<h3> Results from MySQL:<br></h3>";
	$rs = mysql_query($query, $db_connection); #results of query
	
	$i = 0;
	print "<tr align=center>";
	while($i < mysql_num_fields($rs)){
		$meta = mysql_fetch_field($rs, $i);
		print "<td><b>$meta->name</b></td>";
		$i++;
	}
	print "</tr>";

	while($row = mysql_fetch_row($rs)) {
		print "<tr align=center>";
		for($i = 0; $i < mysql_num_fields($rs); $i++){
			if($row[$i] == NULL)
				$var = "N/A";
			else
				$var = $row[$i];

			print "<td>$var</td>";
		}
	}

	print"</table>";

	mysql_close($db_connection);
?>


</body>
</html>