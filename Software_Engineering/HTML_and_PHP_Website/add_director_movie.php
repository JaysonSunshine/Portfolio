<!DOCTYPE html>
<html>
<body>

<h1>Add director to movie</h1>
<form action="add_director_movie.php" method="GET">

<?php

	$db_connection = mysqli_connect("localhost", "cs143", "", "CS143"); #opens connection to database
		if(mysqli_connect_errno()){
			print "Failed to connect to database";
		}

	$result = mysqli_query($db_connection,"SELECT * FROM Movie");
?>

Movie: <select name ="movie">;

<?php
	while($row = mysqli_fetch_row($result)) {
		$title_year = "$row[1]" . " [$row[2]]";
		print '<option value = "' . $title_year . '">' . $title_year . "</option>";
	}
	print "</select>";
		
?>
Director: <select name = "director">;

<?php
	$result = mysqli_query($db_connection,"SELECT * FROM Director ORDER BY last");
	while($row = mysqli_fetch_row($result)) {
		$output = $row[1] . ", " . $row[2] . " [$row[3]]";
		print '<option value = "' . "$output" . '">' . "$output</option>";
	}
	print "</select>";
?>
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

				$last = explode(", ", $_GET[director],2);
				$first = explode(" [", $last[1], 2);

				$query = "select id from Director where last = " . "'" . "$last[0]" . "'" . " and first = " . "'" . "$first[0]" . "'";
				$rs = mysql_query($query, $db_connection_1); #results of query
				
				$did = mysql_fetch_row($rs);

				$sql = "INSERT INTO MovieDirector VALUES($mid[0], $did[0])";
				if(!mysqli_query($db_connection, $sql)){
					print "failed";
				}
				else{
					print "Successfully added director-move relation!";
				}
			}
		}
	}

?>
<br><a href="query.php">Movie Database</a> 


</body>
</html>