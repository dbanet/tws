-- phpMyAdmin SQL Dump
-- version 3.3.2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Erstellungszeit: 06. Oktober 2010 um 22:35
-- Server Version: 5.0.70
-- PHP-Version: 5.2.12-pl0-gentoo

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Datenbank: `lookias`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `clantagtoadressmapping`
--

CREATE TABLE IF NOT EXISTS `clantagtoadressmapping` (
  `id` int(11) NOT NULL auto_increment,
  `tag` varchar(255) collate utf8_unicode_ci NOT NULL,
  `address` varchar(255) collate utf8_unicode_ci NOT NULL,
  `tooltip` varchar(255) collate utf8_unicode_ci NOT NULL,
  `insertTime` varchar(255) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=132 ;

--
-- Daten für Tabelle `clantagtoadressmapping`
--

INSERT INTO `clantagtoadressmapping` (`id`, `tag`, `address`, `tooltip`, `insertTime`) VALUES
(21, 'NNN', 'http://www.normalnonoobs.org/', 'Normal No Noobs', '1285940193'),
(28, 'WormsMalditos', 'http://www.clanwormsmalditos.kit.net/', 'WormsMalditos', '1285940554'),
(29, 'RoH', 'http://www.ropers-of-honor.com/', 'Ropers of Honor', '1285940641'),
(30, 'rA', 'http://ropersaddiction.co.nr/', 'Ropers Addiction', '1285940764'),
(34, 'RS', 'http://realsnipers.tk/', 'Real Snipers', '1285941224'),
(35, 'nlF', 'http://nolimitfreestyle.com/', 'No Limit Freestyle', '1285941254'),
(36, 'TdC', 'http://www.worms-league.com/tdc/', 'The Deadly Clan', '1285941271'),
(37, 'CHivas', 'http://www.tus-wa.com/groups/IL/', 'IL clan owner', '1285941341'),
(38, 'WM`', 'http://www.clanwormsmalditos.kit.net/', 'WormsMalditos', '1285942068'),
(39, 'dP', 'http://www.dpxclan.com/', 'Destroyers of Power', '1285942286'),
(40, 'UIS', 'http://clanuis.net/', 'Universidad Industrial de Santander', '1285942735'),
(42, 'wQw', 'http://www.wqw.xt.pl/', 'Wypasione Qrde Wormsy', '1285943954'),
(43, 'VIW', 'http://www.clanviw.es.tl/', 'Very Important Worms', '1285943980'),
(44, 'nW', 'http://nw.wormsarmageddon.hu/', 'Noname Worms', '1285944861'),
(45, 'ps', 'http://www.tus-wa.com/forums/ps/t', 'ProStars', '1285946762'),
(46, 'proxtarx', 'http://www.tus-wa.com/forums/ps/t', 'ProStars', '1285946771'),
(47, 'ck', 'http://www.tus-wa.com/groups/ck/', 'Crazy Killers', '1285946794'),
(49, 'LL', 'http://the-league-of-losers.blogspot.com/', 'The League of Losers', '1285953805'),
(50, 'CF', 'http://www.team-coldfusion.de/', 'Cold Fusion', '1285953832'),
(51, 'rA`Clan', 'http://ropersaddiction.co.nr/', 'Ropers Addiction', '1285954021'),
(52, 'GrW', 'http://www.tus-wa.com/groups/GrW/', 'Grandes Wormeros', '1285954153'),
(53, 'WoW', 'http://www.world.wa.pl/', 'World of Worms', '1285954523'),
(54, 'RaW', 'http://lrawl.forums1.net/', 'RwA and Wkfb', '1285954551'),
(103, 'DM', 'http://www.clandarkmatter.co.uk/', 'Dark Matter', '1286395078'),
(56, 'TSR', 'http://tsr.spruz.com/', 'The Silent Revelation', '1285956698'),
(57, 'TpM', 'http://clantpm.los-foros.com/', 'The Pro Maggots', '1285957855'),
(58, 'xTpMx', 'http://clantpm.los-foros.com/', 'The Pro Maggots', '1285957868'),
(60, 'wF', 'http://s4.zetaboards.com/The_worm_fusion/index/', 'Worm Fusion', '1285973690'),
(84, 'Korea', 'http://en.wikipedia.org/wiki/Korea', '', '1286058473'),
(67, 'cfc', 'http://cfc.ligaworms.org/', ' 	Clan de Fracasados Coincidentes', '1286028291'),
(77, 'cybershadow', 'http://thecybershadow.net', 'CyberShadow, the W:A maintainer', '1286043834'),
(70, 'JEDI', 'http://xjedix.forum.st/portal.htm', 'JEDI', '1286029394'),
(71, 'xJEDIx', 'http://xjedix.forum.st/portal.htm', 'JEDI', '1286029402'),
(73, 'PoD', 'http://www.prodofdeath.pun.pl/', 'Prod of Death', '1286038625'),
(75, 'WNS', 'http://xwnsx.forumactif.com/', 'WormNet Stars (French clan)', '1286043233'),
(76, 'RAF', 'http://rigidattackforce.com/', 'Rigit Attack Force', '1286043770'),
(102, 'NormalNoNoobs', 'http://www.normalnonoobs.org/', 'Normal No Noobs', '1286381859'),
(79, 'six', 'http://www.tus-wa.com/groups/six/', 'Supreme Immortal Xperts', '1286044006'),
(90, 'thC', 'http://www.tus-wa.com/groups/thC/', 'TetraHydroCannabinol', '1286060934'),
(81, 's2k', 'http://s2k-wa.tk/', 'Skill to Kill', '1286046375'),
(82, 's4f', 'http://s2k-wa.tk/', 'Skill to Kill', '1286046385'),
(83, 'sW', 'http://spiderworms.tk/', 'Spider Worms', '1286046451'),
(91, 'NTP', 'http://ntpclan.webs.com/', 'Nothing To Prove', '1286090304'),
(101, 'HostingBuddy', 'http://worms2d.info/HostingBuddy', '', '1286304220'),
(94, 'SpiderWorms', 'http://spiderworms.tk/', 'Spider Worms', '1286177833'),
(95, 'uW', 'http://ultimate-worms.clan.su/', 'Ultimate Worms', '1286179723'),
(98, 'rKs', 'http://www.tus-wa.com/forums/rks/', 'RocKers', '1286278655'),
(99, 'WebSnoop', 'http://snoop.worms2d.info/', 'WebSnoop', '1286278665'),
(100, 'sK', 'http://serious.kickers.free.fr/', 'Serious Kickers', '1286281535'),
(104, 'DM', 'http://www.clandarkmatter.co.uk/', 'Dark Matter', '1286395085'),
(105, 'xDMx', 'http://www.clandarkmatter.co.uk/', 'Dark Matter', '1286395093'),
(106, 'wcd', 'http://www.wcdclan.de/', 'We Cant Die', '1286395123'),
(107, 'ce', 'http://www.laene.nl/CE', 'Citzen Erased', '1286395253'),
(109, 'ngr', 'http://www.ngrclan.altervista.org/', 'New Generation Ropers', '1286395601'),
(123, 'git', 'http://www.gitclan.com/', 'Good in Theory', '1286396232'),
(111, 'wl', 'http://wormslatinos.es.tl/', 'Worms Latinos', '1286395728'),
(115, 'RW', 'http://clanuis.net/foro-68.html', 'Radio WORMS', '1286395889'),
(114, 'mks', 'http://www.wormsmaniatikos.site50.net/', 'Clan Wormsmaniatikos', '1286395857'),
(116, 'black', 'http://clanxblackx.tk/', 'Clan xBLACKx', '1286395929'),
(117, 'xblackx', 'http://clanxblackx.tk/', 'Clan xBLACKx', '1286395932'),
(118, 'troxby', 'http://clantroxby.foromx.net/', 'CLAN TROXBY', '1286395959'),
(119, 'xtroxbyx', 'http://clantroxby.foromx.net/', 'CLAN TROXBY', '1286395965'),
(120, 'ddd', 'http://www.clan-ddd.tk/', 'Desgraciadamente Desgraciados Dementes', '1286396041'),
(122, 'fmw', 'http://fmw.com.mx/', 'Federacion Mexicana de Worms', '1286396129'),
(124, 'wc', 'http://forum.warbr.com/index.php/board,19.0.html/', 'Worms Champions', '1286396297'),
(125, 'wtf', 'http://wtf.own.cz/', 'Worms Terrorist Force', '1286396332'),
(126, 'wm', 'http://www.clanwormsmalditos.kit.net/', 'WormsMalditos', '1286396365'),
(127, 'CKC', 'http://www.armageddonleague.com/ckc/', 'The Chicken Clan', '1286396406'),
(128, 'tag', 'http://www.ligaworms.org/foro/index.php?board=25.0', 'Tanto atao Gueon', '1286396763'),
(129, 'sheriffbot', 'http://worms2d.info/WormNET_(Worms_Armageddon)#Bots', '', '1286396835'),
(130, 'help', 'http://worms2d.info/WormNET_(Worms_Armageddon)#Bots', '', '1286396866'),
(131, 'psu', 'http://psu.drink.to/', 'Pot Smokers United', '1286397138');
