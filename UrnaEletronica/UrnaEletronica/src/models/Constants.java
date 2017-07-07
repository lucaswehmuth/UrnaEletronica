package models;

import java.util.ArrayList;
import java.util.List;

public class Constants {
    public static final String USER = "g2bsi";
    public static final String PASSWORD = "wa*Uc2";
    public static final String SVADDRESS = "localhost";
    public static final int PORT = 4547;
    
    private static List<Candidato> candidatos;
    
    public Constants(){
        candidatos = new ArrayList<>();
        
	Candidato c0 = new Candidato();
        c0.setNome_candidato("Karl Marx");
        c0.setPartido("TCM");
	c0.setCodigo_votacao(0);
	c0.setNum_votos(0);
        
        Candidato c1 = new Candidato();
        c1.setNome_candidato("Friedrich Engels");
        c1.setPartido("TCM");
	c1.setCodigo_votacao(1);
	c1.setNum_votos(0);
        
        Candidato c2 = new Candidato();
        c2.setNome_candidato("Rosa Luxemburg");
        c2.setPartido("ISDPG");
	c2.setCodigo_votacao(2);
	c2.setNum_votos(0);
        
        Candidato c3 = new Candidato();
        c3.setNome_candidato("Vladimir Lenin");
        c3.setPartido("CPSU");
	c3.setCodigo_votacao(3);
	c3.setNum_votos(0);
        
        Candidato c4 = new Candidato();
        c4.setNome_candidato("Leon Trotsky");
        c4.setPartido("SDP");
	c4.setCodigo_votacao(4);
	c4.setNum_votos(0);
        
        Candidato c5 = new Candidato();
        c5.setNome_candidato("Joseph Stalin");
        c5.setPartido("CPSU");
	c5.setCodigo_votacao(5);
	c5.setNum_votos(0);
        
        Candidato c6 = new Candidato();
        c6.setNome_candidato("Ethel Rosenberg");
        c6.setPartido("CPUSA");
	c6.setCodigo_votacao(6);
	c6.setNum_votos(0);
        
        Candidato c7 = new Candidato();
        c7.setNome_candidato("Mao Tse-tung");
        c7.setPartido("CPChina");
	c7.setCodigo_votacao(7);
	c7.setNum_votos(0);
        
        Candidato c8 = new Candidato();
        c8.setNome_candidato("Fidel Castro");
        c8.setPartido("CPChina");
	c8.setCodigo_votacao(8);
	c8.setNum_votos(0);
        
        Candidato c9 = new Candidato();
        c9.setNome_candidato("Che Guevara");
        c9.setPartido("CPCuba");
	c9.setCodigo_votacao(9);
	c9.setNum_votos(0);
        
        Candidato c10 = new Candidato();
        c10.setNome_candidato("Voto nulo");
        c10.setPartido("NULO");
	c10.setCodigo_votacao(0);
	c10.setNum_votos(0);
        
        candidatos.add(c1);
        candidatos.add(c2);
        candidatos.add(c3);
        candidatos.add(c4);
        candidatos.add(c5);
        candidatos.add(c6);
        candidatos.add(c7);
        candidatos.add(c8);
        candidatos.add(c9);
        candidatos.add(c10);
    }
    
    public List<Candidato> getCandidatos(){
		return candidatos; 
    }
}
