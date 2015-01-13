	<!DOCTYPE html>
<html>
<body>

<h1>Add actor to movie</h1>
<form action="add_actor_movie.php" method="GET">

<?php

	$db_connection = mysqli_connect("localhost", "cs143", "", "CS143"); #opens connection to database
		if(mysqli_connect_errno()){
			print "Failed to connect to database";
		}
		else{
			$result = mysqli_query($db_connection,"SELECT * FROM Movie");
		}
?>

Movie: <select name ="movie">;

<?php
	while($row = mysqli_fetch_row($result)) {
		$title_year = "$row[1]" . " [$row[2]]";
		print '<option value = "' . $title_year . '">' . $title_year . "</option>";
	}
	print "</select>";
		$result = mysqli_query($db_connection,"SELECT * FROM Actor ORDER BY last");
?>
Actor: <select name = "actor">;

<?php
	while($row = mysqli_fetch_row($result)) {
		$output = $row[1] . ", " . $row[2] . " [$row[4]]";
		print '<option value = "' . "$output" . '">' . "$output</option>";
	}
	print "</select>";
?>

<br>Role: <input type = "text" name="role"><br>

<input type="submit" value="Submit">
</form>

<?php
	if(!empty($_GET)){
		$db_connection = mysqli_connect("localhost", "cs143", "", "CS143"); #opens connection to database
		if(mysqli_connect_errno()){
			print "Failed to connect to database";
		}
		else{
			$db_connection_1 = mysql_connect("localhost", "cs143", ""); #opens connection to database
			mysql_select_db("CS143", $db_connection_1); #opens CS143 database

			$arr = explode(" [", $_GET[movie],2);

			$query = "select id from Movie where title = " . "'" . "$arr[0]" . "'";

			$rs = mysql_query($query, $db_connection_1); #results of query
			if($rs != FALSE){
				$mid = mysql_fetch_row($rs);

				$last = explode(", ", $_GET[actor],2);
				$first = explode(" [", $last[1], 2);

				$query = "select id from Actor where last = " . "'" . "$last[0]" . "'" . " and first = " . "'" . "$first[0]" . "'";

				$rs = mysql_query($query, $db_connection_1); #results of query
				
				$aid = mysql_fetch_row($rs);

				$sql = "INSERT INTO MovieActor VALUES($mid[0], $aid[0], '$_GET[role]')";

				if(!mysqli_query($db_connection, $sql)){
					print "failed";
				}
				else{
					print "Successfully added actor-move relation!";
				}
			}
		}
	}
?>
<br><a href="add_director_movie.php">Add director to movie</a> 

<br><a href="query.php">Movie Database</a> 


</body>
</html>