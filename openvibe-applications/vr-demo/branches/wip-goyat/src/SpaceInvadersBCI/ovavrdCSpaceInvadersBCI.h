#ifndef __OpenViBEApplication_CSpaceInvadersBCI_H__
#define __OpenViBEApplication_CSpaceInvadersBCI_H__

#define ALIENFLASH 1 //� 1 pour faire flasher les aliens et non le fond
#define MOBILE 1 //� 1 pour que les aliens se d�placent
#define MISSILES 1 //� 1 pour que les aliens tirent des missiles
#define DIAGONALE 0 //� 1 pour que les aliens identiques soient sur une diagonale plut�t qu'une ligne
//taille de la matrice d'aliens
#define Nalien 6 
#define Malien 5
//taille de la matrice de flash
#if ALIENFLASH
#define Nflash Nalien
#define Mflash Malien
#else
#define Nflash 6
#define Mflash 5
#endif


//#define DUREEFLASHMICRO 100000
//#define DUREESOAMICRO 125000

#include <Ogre.h>        ////////////////////////difference
using namespace Ogre;        ////////////////////////difference
#include "PseudoMatrice.h"
#include "MatriceAliens.h"
#include "MatriceFlash.h"
#include "MatriceAlienFlash.h"
#include "Tank.h"
#include "Missile.h"
#include "Roquette.h"
#include "LaserBaseH.h"
#include "LaserBaseV.h"
#include "../ovavrdCOgreVRApplication.h"

enum StimulationState
{
	State_None,
	State_Flash,
	State_NoFlash,
	State_RepetitionRest,
	State_TrialRest
};

	
namespace OpenViBEVRDemos {
 
	class CSpaceInvadersBCI : public COgreVRApplication
	{
		public:
			CSpaceInvadersBCI();

		private:
			virtual bool initialise(void);
			virtual bool process(double timeSinceLastProcess);

		private :
			void loadGUI(void);
					
			void initFirstVariables();
			void readConfigFile();
			void initSecondVariables();
			
			void processStageJeu(double timeSinceLastProcess);
			void processStageApprentissage(double timeSinceLastProcess);
			//
			void processGestionFlashP300(double timeSinceLastProcess);
			//
			void moveMatrix();
			bool m_endMatrixWalk;
			void moveTank();
			void moveLaserBase();
			void generateMissiles();
			void moveMissiles();
			bool m_collisionMissileTank;
			void moveRoquettes();
			
			bool ConditionVictoireStayNAlien(int nRestant);
			
			void viseurSurAlien(int i,int j);
			void detruireAlien(int i,int j);
			void reinitialisation(void);
			void fermeture(void);
			bool algoSequenceLigne(void);
			bool algoSequenceColonne(void);
			
			bool algoSequenceUpdate(void);
			bool algoSequenceGen(void);
			
						//void timerListener(void);//gestion des flashs, sorties port parallele, cr�ation de missiles, gestion des roquettes
#if !ALIENFLASH
			SceneNode* alienDevantCaseFlash(int i, int j,Vector3 Marge);
#endif

			bool LignesFull;
			bool ColonnesFull;

			int m_iStage;                      //phase de jeu
			bool m_bApprentissageFini;   // vrai si on a d�ja pass� une phase d'apprentissage
			int m_iVie;
			double m_dFeedback;                //valeur analogique recue par le vrpn
			double m_dLastFeedback;            //feedback precedent

			double m_dMinimumFeedback;         //!<Minimum feedback value ever received.

			enum
			{
				Stage_None,
				Stage_Pret,   
				Stage_Jeu,   
				Stage_Perdu, 
				Stage_Gagne, 
				Stage_Apprentissage,
			};

			bool keyPressed(const OIS::KeyEvent& evt);
			bool keyReleased(const OIS::KeyEvent& evt);

			double m_dBetaOffset;
			int m_iBetaOffsetPercentage;
		
			
		protected:
			Real mMOVETANK;            // Constantes de mouvement pour le tank
			Real mMoveAlien;            // Constantes de mouvement pour les invaders
			Real mMoveMissile;            // Constantes de mouvement pour les missiles
			Real mMoveRoquette;            // Constantes de mouvement pour les roquettes
			//int mOutPort1;				//adresse du port de sortie
			int mValeurPortOutTAG;				//valeur � �crire dans le port

			std::list<int> LigneFlash; //les derni�res lignes flash�es de la sequence de flashs
			std::list<int> ColonneFlash; //derni�res colonnes flash�es de la sequence de flashs

			Vector3 mDirecTank;			//vecteur de direction pour le tank
			Vector3 mDirecLaserBaseH;
			Vector3 mDirecLaserBaseV;

			//FlashPhase mPhase; //pour la sequence de flash
			Timer timerFlash;			//timer pour les dur�es des flashs
			Timer timerInterTrial;

			Timer m_timerMissile;			//timer pour la cr�ation de missiles
			Timer m_timerRoquette;			//timer pour la cr�ation de roquettes
			int CompteurRepetition;
			double LigneSelection;		//num�ro de la prochaine ligne � d�truire, receptionn� par vrpn
			double ColonneSelection;		//num�ro de la prochaine colonne � d�truire, receptionn� par vrpn
			bool ligneSelected;			//ligne re�ue pour destruction d'alien
			bool ColonneSelected;		//colonne re�ue pour destruction d'alien
			Vector2 aDetruire;       //alien en train d'exploser
			Timer timerExplosion;    //timer pur la dur�e d'une explosion d'alien
			int DUREEFLASHMICRO;
			int DUREESOAMICRO;
			int DUREEINTERTRIALMICRO;
			int REPETITIONCOUNT;
			int PERIODMISSILE;
			int PERIODROQUETTE;

			//gestion des flashs
			bool m_bStartReceived;
			long int m_liTrialStartTime;
			int m_iTrialCount;
			int m_iTrialIndex;
			long int m_liTrialDuration;
			long int m_liInterTrialDuration; //DUREEINTERTRIALMICRO
			long int m_liRepetitionDuration;
			long int m_liInterRepetitionDuration;
			long int m_liFlashDuration; //DUREEFLASHMICRO
			long int m_liNoFlashDuration; //DUREESOAMICRO
			StimulationState m_oLastState;
			//savegarde des s�quences de flash : 
			std::list<int> ListFlash; //sequence � flasher
			std::vector<int> m_vSequence;

			
#if MISSILES
			int m_numeroMissile;		//num�ro incr�ment� � chaque cr�ation de missile
			std::list<Missile*> m_ListeMissiles;   //liste des missiles d�j� tir�s
#endif
			int m_numeroRoquette;		//num�ro incr�ment� � chaque cr�ation de roquette
			std::list<Roquette*> m_ListeRoquettes;   //liste des roquettes d�j� tir�s
			int FLASHDEBUG;
			Tank *mTank;
			LaserBaseH *m_LaserBaseH;
			LaserBaseV *m_LaserBaseV;

#if ALIENFLASH==0
			MatriceAliens *mMatAlien;
			MatriceFlash *mMatFlash;
#else
			MatriceAlienFlash *mMatAlien;
			MatriceAlienFlash *mMatFlash;
#endif

	};
};
#endif //__OpenViBEApplication_CSpaceInvadersBCI_H__
