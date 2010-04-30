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
	`classification`	varchar(225),
	`width`			INT(5),
	`height`		INT(5),
	`image`			LONGBLOB,
	`dataset_id`		INT(11),
	`format_id`		INT(11),
	FOREIGN KEY (`dataset_id`) REFERENCES dataset(`id`),
	FOREIGN KEY (`format_id`)  REFERENCES format(`id`),
	PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `format`;
CREATE TABLE `format` (
	`id` 			INT(11) NOT NULL AUTO_INCREMENT,
	`type` 			VARCHAR(225),
	PRIMARY	KEY (`id`)
)
