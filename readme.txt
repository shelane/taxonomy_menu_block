The aim taxonomy menu

Functionaliteiten:

Module gebouwd met het oog op het eenvoudig en efficiënt genereren van een meertalige taxonomy menu zonder caching. Er is ook een active state en active-trail voorzien zodanig dat op een eenvoudige manier een accordeon menu kan gemaakt worden. 


Installatie:

Na enablen van module: 
1) ga naar admin/config/development/the_aim_taxonomy_menu
2) selecteer de gewenste vocabulary
3) ga naar admin/structure/block zet The Aim Taxonomy menu op de gewenste plaats

Theming:

Override theme function theme_taxonomy_menu_block($variables) in your template.php. 


Troubleshooting:

De module gaat ervan uit dat de taxonomy meertalig is ingesteld. Bijgevolg zal hij enkel werken wanneer de translation mode van je taxonomy op: 'Translate. Different terms will be allowed for each language and they can be translated.' staat. Ook mogen(zullen niet getoond worden) terms niet language neutral zijn. 

Todo:
- Meerdere taxonomy menu's mogelijk maken => Done (Sam) - 10/06/2012
- i18n_taxonomy_get_tree functie wordt twee maal opgeroepen ( performance : dit zou normaal met caching moeten opgevangen worden dus geen dringende issue )