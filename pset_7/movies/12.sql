SELECT title FROM movies
    JOIN stars ON movies.id = stars.movie_id
    JOIN people ON people.id = stars.person_id
    WHERE name IN ("Johnny Depp", "Helena Bonham Carter")
    GROUP BY movie_id
    HAVING COUNT (DISTINCT person_id) = 2;
