DROP TABLE IF EXISTS `dataset`;
CREATE TABLE `dataset` (
	`id` 		INT(11) NOT NULL AUTO_INCREMENT,
	`name`  	VARCHAR(225) NULL,
	`source`	VARCHAR(225),
	`description`	text,
	PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `item`;
CREATE TABLE `item` (
	`id`			INT(11) NOT NULL AUTO_INCREMENT,
	`classification_id`	INT(11),
	`width`			INT(5),
	`height`		INT(5),
	`image`			LONGBLOB,
	`dataset_id`		INT(11),
	`format_id`		INT(11),
	FOREIGN KEY (`dataset_id`) REFERENCES dataset(`id`),
	FOREIGN KEY (`format_id`)  REFERENCES format(`id`),
	FOREIGN KEY (`classification_id`) REFERENCES classification(`id`),
	PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `classification`;
CREATE TABLE `classification` (
	`id`			INT(11) NOT NULL AUTO_INCREMENT,
	`name`			varchar(225),
	`u_low`			INT(3),
	`u_high`		INT(3),
	`v_low`			INT(3),
	`v_high`		INT(3),
	PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `format`;
CREATE TABLE `format` (
	`id` 			INT(11) NOT NULL AUTO_INCREMENT,
	`type` 			VARCHAR(225),
	PRIMARY	KEY (`id`)
)
