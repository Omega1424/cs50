-- Keep a log of any SQL queries you execute as you solve the mystery.
.schema -- to know what data is available in crime scene reports and other tables

-- to find further information about the crime using date and location of crime
SELECT id, description FROM crime_scene_reports WHERE month = 7 AND day = 28 AND year = 2020 AND street = 'Chamberlin Street';
-- Now i know to check the transcripts from the interviews of 3 people

SELECT id, transcript FROM interviews WHERE month = 7 AND day = 28 AND year = 2020;
-- There are 6 interviews on the same date, but 3 of them seem unrelated to the crime. The id's of the matching transcripts are 161,162,163
-- I need to check the courthouse parking lot footage from 10.15 to around 10.25am
-- I need to check ATM transactions for that date around the morning at Fifer Street
-- I need to check phone calls table to find the accomplice using the date, time and duration(less than 1 minute) of the call.
-- I need to check flights table for earliest flight out of fiftyville on 29 July 2020.

SELECT activity, license_plate FROM courthouse_security_logs WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND minute>=15 AND minute<=25;
-- There are 8 possible cars exiting within this timeframe.
-- I can check the people table for the 8 names and passport numbers and phone numbers of the 8 car owners.

SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw');
-- There are 8 person_id's that match this


SELECT id FROM people WHERE people.license_plate IN(SELECT license_plate FROM courthouse_security_logs WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND minute>=15 AND minute<=25) AND people.id IN(SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw'));
-- Now there are 4 possible people who were at the atm and at the courthouse exiting, meeting the known conditions.
-- I will save the 4 people's passport numbers and phone numbers to avoid making the query too complex
-- I must check if any of these 4 were on the flight on 29 july 2020 out of fiftyville.

SELECT name, destination_airport_id, hour, minute FROM flights JOIN passengers ON passengers.flight_id = flights.id JOIN people ON people.passport_number = passengers.passport_number WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville') AND flights.id IN (SELECT flight_id FROM passengers WHERE people.passport_number IN(7049073643, 8496433585, 3592750733, 5773159633)) AND month = 7 AND day = 29 AND year = 2020 ORDER BY hour, minute AND people.passport_number IN(7049073643, 8496433585, 3592750733, 5773159633);
-- Now we know the people who were on flights from Fiftyville on 29 July 2020 who match the passport numbers from the people at the atm and the courthouse(license plate).
-- The first 2 people were on the first flight so the criminal is among Ernest and Danielle.

SELECT name, phone_number FROM people WHERE name IN ('Danielle', 'Ernest');
-- Save each of their phone numbers individually for queries
-- I must search in the phone calls table on who made a phone call on that date under 1 minute.

SELECT caller, receiver FROM phone_calls WHERE(caller = '(389) 555-5198' OR receiver = '(389) 555-5198' OR caller = '(367) 555-5533' OR receiver = '(367) 555-5533') AND duration < 60 AND month = 7 AND day
-- I queried for both caller and reciever since we are not sure if the criminal received or made the call.
-- We now know the criminal is Ernest as he is the only one who made a call with the right duration and date. I can check the accomplices name using the phone number and the people table.

SELECT name FROM people WHERE phone_number = '(375) 555-8161';
-- I found the accomplice's name by checking the phone number of the person who was on the call with Ernest in the people table.

SELECT full_name, city FROM airports JOIN flights ON flights.destination_airport_id = airports.id JOIN passengers ON flights.id = passengers.flight_id JOIN people ON passengers.passport_number = people.passport_number WHERE name = 'Ernest';
-- Ernest escaped to Heathrow Airport in London.