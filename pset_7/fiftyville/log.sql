-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT description FROM crime_scene_reports WHERE month = 7 AND day = 28 AND year = 2021 AND street = "Humphrey Street";
-- Theft took place at 10:15 AM at the Humphrey Street bakery. Interviews conducted with 3 witnesses mention bakery

SELECT name, transcript FROM interviews WHERE month = 7 AND day = 28 AND year = 2021;
-- Transcripts:
    -- EMMA: Bakery owner, whispering into a phone for half an hour, never bought anything
    -- RUTH: 10 mins car of theft bakery parking lot and drive away... look at security footage!
    -- EUGENE: someone he recognized; earlier in morning walking by the ATM on Leggett Street and saw thief there withdrawing some money
    -- RAYMOND: Called someone who talked for less than a minute; earliest flight out of Fiftyville tomorrow and purchase the flight ticket

SELECT caller, receiver, duration FROM phone_calls WHERE month = 7 AND day = 28 AND year = 2021;
-- Bunch of phone calls but nothing specific

SELECT account_number, amount FROM atm_transactions WHERE month = 7 AND day = 28 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";
-- Tied to account numbers which are tied to people IDs, no one spent over $80

SELECT hour, minute, activity, license_plate FROM bakery_security_logs WHERE month = 7 AND day = 28 AND year = 2021 AND hour = 10 AND minute >= 15 AND minute <= 25;
-- Promising 8 results of license plates

SELECT * FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE month = 7 AND day = 28 AND year = 2021 AND hour = 10 AND minute >= 15 AND minute <= 25;
-- INITIAL SUSPECTS + IDs from car:
    -- Vanessa (221103), Barry (243696), Iman (396669), Sofia (398010), Luca (467400), Diana (514354), Kelsey (560996), Bruce (686048)

SELECT * FROM people
    JOIN bank_accounts ON people.id = bank_accounts.person_id
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE month = 7 AND day = 28 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";
-- SUSPECTS narrowed down:
    -- Bruce ($50), Diana ($25), Iman ($20), Luca ($48)

SELECT * FROM people
    JOIN phone_calls ON phone_calls.caller = people.phone_number
    WHERE (people.id = 686048 OR people.id = 514354 OR people.id  = 467400 OR people.id = 396669) AND month = 7 AND day = 28 AND year = 2021;
-- SUPSECTS narrowed down:
    -- Bruce made 4 calls, Diana made 1. Most likely Bruce just have to confirm now. Phone number is (367) 555-5533, passport is 5773159633, license plate is 94KL13X.

SELECT * FROM phone_calls WHERE caller = "(367) 555-5533" AND month = 7 AND day = 28 AND year = 2021;
-- Receivers of his calls are (375) 555-8161, (344) 555-9601, (022) 555-4052, (704) 555-5790. (375) 555-8161 is his accomplice who he talked to for less than a minute.

SELECT * FROM people WHERE phone_number = "(375) 555-8161";
-- Robin is his accomplice!

SELECT * FROM flights
    JOIN passengers ON passengers.flight_id = flights.id
    WHERE passport_number = 5773159633;
-- Destination airport id is 4, so...

SELECT * FROM airports WHERE id = 4;
-- LaGuardia Airport in New York City!