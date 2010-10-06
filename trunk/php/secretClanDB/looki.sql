-- phpMyAdmin SQL Dump
-- version 3.2.4
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Erstellungszeit: 01. Oktober 2010 um 12:28
-- Server Version: 5.1.41
-- PHP-Version: 5.3.1

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Datenbank: `looki`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `clantagtoadressmapping`
--

CREATE TABLE IF NOT EXISTS `clantagtoadressmapping` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tag` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `address` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `tooltip` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `insertTime` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=19 ;

--
-- Daten für Tabelle `clantagtoadressmapping`
--

INSERT INTO `clantagtoadressmapping` (`id`, `tag`, `address`, `tooltip`, `insertTime`) VALUES
(7, 'GG', 'http://www.yahoo.de', 'Google Inc.', '1285927344'),
(12, 'GG', 'http://www.google.de', 'Google Inc.', '1285927367'),
(13, 'GG', 'http://www.google.de', 'Google Inc.', '1285927368'),
(18, 'GG', 'http://www.google.de', 'Google Inc.', '1285927369');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
