<!DOCTYPE html>
<html>
<body>

<?php
	if(!empty($_GET)){
		$db_connection = mysqli_connect("localhost", "cs143", "", "CS143"); #opens connection to database
		if(mysqli_connect_errno()){
			print "Failed to connect to database";
		}
		else{
			$query = "select * from Movie where id = $_GET[id]";
			$rs = mysqli_query($db_connection, $query); #results of query
			if($rs != FALSE){
				$row = mysqli_fetch_row($rs);
				$title = "$row[1]";
				print "<h1>$title [$row[2]]</h1>";
				print "<br>Rating: $row[3] <br> Producer: $row[4]";
			}
			$query_movie = "select did from MovieDirector where mid = $_GET[id]";
			$result = mysqli_query($db_connection, $query_movie); #results of query
			if($result != FALSE){
				$row_movie = mysqli_fetch_row($result);
				$query_movie = "select first, last from Director where id = $row_movie[0]";
				$rs = mysqli_query($db_connection, $query_movie); #results of query
				if($rs != FALSE){
					$row_movie = mysqli_fetch_row($rs);
					print "<br> Director: $row_movie[0] $row_movie[1]";
				}
			}
			$query = "select genre from MovieGenre where mid = $_GET[id]";
			$rs = mysqli_query($db_connection, $query); #results of query
			$counter = 0;
			print "<br>Genre: ";
			if($rs != FALSE){
				while($row = mysqli_fetch_row($rs)) {
					if($counter != 0){
						print ", ";
					}
					print "$row[0]";
					$counter ++;
				}
			}
			$query = "select aid,role from MovieActor where mid = $_GET[id]";
			$rs = mysqli_query($db_connection, $query); #results of query
			print "<h3>Actors in $title</h3>";
			while($row = mysqli_fetch_row($rs)) {
				$actor = "select first,last from Actor where id = $row[0]";
				$rs_actor = mysqli_query($db_connection, $actor); #results of query
				$row_actor = mysqli_fetch_row($rs_actor);
				print "<br><a href=" .'"' . "browse_actor.php?id=$row[0]" . '">' . "$row_actor[0] $row_actor[1]</a> as " . "'" . "$row[1]" . "'";
			}

			$avg = "select avg(rating), count(*) from Review where mid = $_GET[id]";
			$rs = mysqli_query($db_connection, $avg); #results of query
			if($rs != FALSE){

				$row = mysqli_fetch_row($rs);
				if($row[0] != NULL){
					print "<h3>Reviews</h3><br>Average Score = $row[0] / 5 by $row[1] reviewers.<br>";
				
					$review = "select * from Review where mid = $_GET[id]";
					$rs = mysqli_query($db_connection, $review); #results of query
					while($row = mysqli_fetch_row($rs)){
						print "At $row[1], $row[0] gave $title a rating of $row[3]/5 and wrote:<br> $row[4]<br>";
					}
					print '<br><a href="add_comments.php">Add a movie review</a><br>';
				}
				else{
					print "<h3>Reviews</h3>";
					print '<a href="add_comments.php">Add a movie review</a><br>';
				}
			}
		}
	}
	else{
		print "<h1>Browse Movies</h1>";
	}
?>
<form action="search.php" method="GET">
<br>Search: <input type = "text" name="search"><br>
<input type="submit" value="Submit">
</form>

<br><br><a href="query.php">Movie Database</a> 


</body>
</html>