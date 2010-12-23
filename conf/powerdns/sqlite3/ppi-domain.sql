-- Create the domain name

DELETE from records;
DELETE from domains;

INSERT INTO domains (id, name, type) values (0, 'ppi', 'NATIVE');
