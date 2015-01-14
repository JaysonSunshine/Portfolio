<!DOCTYPE html>
<html>
<body>

<h1>Search for actors or movies</h1>

<form action="search.php" method="GET">
Search: <input type = "text" name="search"><br>
<input type="submit" value="Search">
</form>

<br><a href="add_actor_director.php">Add a new actor or director</a> <br><br>
<a href="add_movie.php">Add a new movie</a> <br><br>
<a href="add_comments.php">Add a new movie comment</a> <br><br>
<a href="query.php">Return home</a> <br><br>

<?php
	if(!empty($_GET)){
		print '<Search results for "' . "$_GET[search]" . '"<br><br>';
		$db_connection = mysqli_connect("localhost", "cs143", "", "CS143"); #opens connection to database
		if(mysqli_connect_errno()){
			print "Failed to connect to database";
		}
		else{
			$db_connection_1 = mysql_connect("localhost", "cs143", ""); #opens connection to database
			mysql_select_db("CS143", $db_connection_1); #opens CS143 database

			$arr = explode(" ", trim($_GET[search]));
			$clauses = array();
			foreach($arr as $part){
				$clauses[] = "(last like '%" . mysql_real_escape_string($part) . "%'" . " or first like '%" . mysql_real_escape_string($part) . "%')";
			}
			$clause = implode(' AND ', $clauses);
			$query = "select first, last, dob, id from Actor where $clause" ;

			$rs = mysql_query($query, $db_connection_1); #results of query

			while($row = mysql_fetch_row($rs)) {
				print 'Actor: <a href="' . "browse_actor.php?id=$row[3]" . '">' . "$row[0] $row[1] [" . "$row[2]" . "]</a><br>";
			}
			print "<br>";
			foreach($arr as $part){
				$clauses_2[] = "(title like '%" . mysql_real_escape_string($part) . "%')";
			}
			$clause = implode(' AND ', $clauses_2);
			$query = "select title, year, id from Movie where $clause";

			$rs = mysql_query($query, $db_connection_1); #results of query
			if($rs != FALSE){
				while($row = mysql_fetch_row($rs)) {
					print 'Movie: <a href="' . 'browse_movie.php?id=' . "$row[2]" . '">' . "$row[0] [$row[1]]</a><br>";
				}
			}
		}
	}
?>


</body>
</html>