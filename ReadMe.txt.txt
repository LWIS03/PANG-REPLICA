Integrants:
	-Diego Rodriguez Milagro
	-Lluis Itarte Sahun

Anotació:
	- Al Binari el só del joc no funciona degut als directoris, Per jugarlo amb so accedeix a: \projecte\2DGame\02-Bubble\Release\02-Bubble.exe, sentim les molesties.

Funcionalitats implementades:
	-Estructura basica de 4 pantalles:
		-Menu principal
		-Jugar
		-Instruccions
		-Credits (al morir o completar els 3 nivells)
	-3 nivells amb les configuracions demanades
	-Bombolles afectades per la gravetat y que reboten per lescenari amb els blocs i barreres. Si una bombolla toca al jugador, aquest perd una vida.
	-Ganxo disparable que destrueix bombolles i barreres destructibles
	-3 Power-ups produits per la destruccio de bombolla:
		-Sticky: l'arma es queda enganxada als blocs no destructibles
		-Double: el jugador pot disparar un segon ganxo un cop el primer ja s'ha disparat
		-Stop time: Les bombolles es paren al lloc on estan durant 4 segons. Durant l'ultim segon les bombolles parpellegen
	-Objectes de menjar que el jugador pot recollir per guanyar 1000 o 2000 punts
	-Combos de rebentar bombolles i recollir objectes de menjar
	-El jugador pot pujar i baixar escales
	-Cada nivell te 100 segons per ser superat. En cas contrari el jugador perd una vida
	-Quan el jugador es queda amb 0 vides, aquest mor i es mostra la pantalla de credits
	-interficie grafica que mostra les vides restants, el temps restant, el nom de la fase, els power ups (excepte el de parar el temps ja que es mostra que s'esta acabant pel parpelleig de les bombolles) i si el god mode esta activat
	-Tecles per saltar contingut, guanyar power ups y activar/desactivar el god mode:
		-0: Menu principal
		-1: Nivell 1
		-2: Nivell 2
		-3: Nivell 3
		-4: Credits
		-T: Power up Sticky
		-Y: Power up Double
		-U: Power up Stop time
		-G: activar/desactivar el god mode
	-Sons i musica
	-Interactivitat amb les entitats del joc:
		-Explosio bombolles
		-Final power up de Stop time
		-Destruccio blocs destructibles

Instruccions del joc:
	L'executable del joc esta situat a la carpeta binari (com s'ha demanat). En obrir-lo s'entra a la pantalla principal.
	En fer click a la tecla S es pasa a la pantalla d'instruccions.
	En esperar 10 segons o fer click novament a la tecla S es pasa al primer nivell.
	En els nivells el personatge es mou amb les fletxes dreta i esquerra i puja i baixa escales amb les fletxes d'adalt i abaix. El personatge dispara amb la tecla S.
	En destruir totes les bombolles es pasa al nivell 2, en el que al destruir totes les bombolles es pasara al nivell 3.
	En el tercer nivell, quan es destrueixin totes les bombolles es passara a la pantalla de credits.
	En la pantalla de credits es pot fer click a la tecla S per tornar a la pantalla principal.

Aspectes a remarcar:
	En el joc que entreguem hi han certs bugs/mecaniques amb mal funcionament que per temes de temps hem preferit prioritzar implementar mes funcionalitats. Els que em pogut observar son:
		-Les colisions de les bombolles fallen en certs moments i aixo fa que puguin fer canvis de direccio  inesperats o que en algun cas esporadic es puguin arribar a sortir del mapa, fent aixi que no es pugui superar el nivell.
		-La part mes alta de l'arma surt desde el cap del personatge, pero la part mes baixa surt sempre desde abaix. Aquesta mecanica no afecta el primer i segon nivell pero fan que el tercer nivell sigui molt facil ja que en pujar-te per les escales pots destruir totes les bombolles sense cap mena de perill