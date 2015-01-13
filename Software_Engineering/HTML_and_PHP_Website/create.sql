create table Movie(id int PRIMARY KEY CHECK(id >= 0), title varchar(100) NOT NULL, year int CHECK(year >= 1850), rating varchar(100), company varchar(50)) ENGINE = InnoDB;
--PRIMARY KEY indexes (faster searches), requires unique entries, and disallows null entires; id must be non-negative
--We want a title for every movie
--We want years of movies to be more modern than 1849
create table Actor(id int PRIMARY KEY CHECK(id >= 0), last varchar(20), first varchar(20), sex varchar(6), dob date NOT NULL CHECK(dob >= 18000101), dod date CHECK(dod > dob)) ENGINE = InnoDB;
--Actor.id same reasoning as Movie.id
--We want a birthdate for every actor by 1800 or more modern, and dob, if it exists must be after dob
create table Director(id int PRIMARY KEY CHECK(id >= 0), last varchar(20), first varchar(20), dob date CHECK(dob >= 1800-01-01), dod date CHECK(dod > dob)) ENGINE = InnoDB;
--Director.id same reasoning as Movie.id
--Same reasoning on dob and dod
create table MovieGenre(mid int, genre varchar(20), FOREIGN KEY (mid) REFERENCES Movie(id)) ENGINE = InnoDB;
--mid is linked to Movie.id; we want all movies in MovieGenre to exist in our collection of movies, ie Movie
create table MovieDirector(mid int, did int, FOREIGN KEY (mid) REFERENCES Movie(id), FOREIGN KEY (did) REFERENCES Director(id)) ENGINE = InnoDB;
--mid is linked to Movie.id; same as above
--did is linked to Director.id; same as above
create table MovieActor(mid int, aid int, role varchar(50), FOREIGN KEY (mid) REFERENCES Movie(id), FOREIGN KEY (aid) REFERENCES Actor(id)) ENGINE = InnoDB;
--Same reasoning on referential integrity as above
create table Review(name varchar(20), time timestamp, mid int, rating int CHECK(rating >= 0 AND rating <= 5), comment varchar(500), FOREIGN KEY (mid) REFERENCES Movie(id)) ENGINE = InnoDB;
--rating is restricted to 0-5
create table MaxPersonID(id int CHECK(id >= 0)) ENGINE = InnoDB;
--id restricted to non-ngative (sould also be greater than greast M.id but can't reference other tables in check constraints)
create table MaxMovieID(id int CHECK(id >= 0)) ENGINE = InnoDB;	
--same as above