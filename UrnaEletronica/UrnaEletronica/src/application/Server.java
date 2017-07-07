package application;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;
import java.net.InetSocketAddress;
import models.Candidato;
import models.Constants;

public class Server {
    
    private static Constants constants;
    private static List<Candidato> candidatos;
    
    public static void main(String[] args) throws IOException{
        constants = new Constants();
        candidatos = constants.getCandidatos();
        String opcao;
        ServerSocket serverSocket = new ServerSocket(40002);
        System.out.println("Servidor iniciado com sucesso!");
    	
        try{
            while (true) {
            	Socket optionSocket = serverSocket.accept(); 
            	opcao = getOpcao(optionSocket);
            	System.out.println("Opcode_cliente = " + opcao);
            	switch (opcao) {
                    //Voto normal
                    case("1"):
                        vote_candidate(optionSocket);
                        sendConfirmationVote(optionSocket);
                        break;
                    
                    //Voto branco
                    case("2"):
                        white_vote(optionSocket);
                        break;
                        
                    //Voto nulo
                    case("3"):
                        null_vote(optionSocket);
                        break;
                    
                    //Enviar candidatos
                    case("999"):
                        send_candidates(optionSocket);
                        break;
                        
                    //Computar vencedor
                    case("888"):
                        count_votes(optionSocket);
                        break;
            	}
            }
        } 
        finally {
            serverSocket.close();
        }
    }
    
    private static void sendConfirmationOpcode(final Socket socket) throws IOException {	
            Thread thread = new Thread(){
                @Override
                public void run() {
                    try {
                        OutputStreamWriter writer = new OutputStreamWriter(socket.getOutputStream(), "UTF-8"); //Escrever no socket
                        System.out.println("Opcode recebido - Confirmacao enviada para o cliente");
                        writer.write("Opcode recebido pelo servidor!"); 
                        writer.flush();                    
                    } catch (IOException e){
                        e.printStackTrace();
                    }
                    return;
                }
            };
            thread.run();
    }
    
    private static void sendConfirmationVote(final Socket socket) throws IOException {	
            Thread thread = new Thread(){
                @Override
                public void run() {
                    try {
                        OutputStreamWriter writer = new OutputStreamWriter(socket.getOutputStream(), "UTF-8"); //Escrever no socket
                        System.out.println("Voto recebido - Confirmacao enviada para o cliente");
                        writer.write("Voto recebido pelo servidor!"); 
                        writer.flush();                    
                    } catch (IOException e){
                        e.printStackTrace();
                    }finally{
                        closeSocket();
                    }
                    return;
                }

                private void closeSocket(){
                    try {
                        socket.close();
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }
                }
            };
            thread.run();
    }
    
    private static void vote_candidate(final Socket socket) throws IOException {
        FutureTask<String> thread = new FutureTask<String>(new Callable<String>() {
            @Override
            public String call() throws Exception {
                BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream(), "UTF-8"));
                System.out.println("Cliente enviou candidato escolhido");  
                int voto = Integer.parseInt(reader.readLine());
                System.out.println("voto = " + voto);
                
                for(int i=0;i < candidatos.size();i++){
                    if(voto == candidatos.get(i).getCodigo_votacao()){
                        candidatos.get(i).somaVoto();
                        System.out.println("Seu voto foi computado com sucesso! (" + voto + ")");
                        break;
                    }
                    else if(i + 1 == candidatos.size()){
                        System.out.println("Candidato não encontrado! (" + voto + ")");
                    }
                }
                return "OK";
            }
    	});
        thread.run();
    }
    
    private static void white_vote(final Socket socket) throws IOException {
        int i, max = 0, winner = 0;
        
        for(i=0;i<candidatos.size();i++){
            if(candidatos.get(i).getNum_votos() > max){
                winner = i;
                max = candidatos.get(i).getNum_votos();
            }
        }
        candidatos.get(winner).somaVoto();
        
        Thread thread = new Thread(){
                @Override
                public void run() {
                    try {
                        OutputStreamWriter writer = new OutputStreamWriter(socket.getOutputStream(), "UTF-8"); //Escrever no socket
                        System.out.println("Voto em branco recebido - Confirmacao enviada para o cliente");
                        writer.write("Voto recebido pelo servidor!"); 
                        writer.flush();                    
                    } catch (IOException e){
                        e.printStackTrace();
                    }
                    return;
                }
            };
            thread.run();
    }
    
    private static void null_vote(final Socket socket) throws IOException {
        candidatos.get(9).somaVoto();
        System.out.println("Voto nulo computado com sucesso!");
        
        Thread thread = new Thread(){
                @Override
                public void run() {
                    try {
                        OutputStreamWriter writer = new OutputStreamWriter(socket.getOutputStream(), "UTF-8"); //Escrever no socket
                        System.out.println("Voto nulo recebido - Confirmacao enviada para o cliente");
                        writer.write("Voto recebido pelo servidor!"); 
                        writer.flush();                    
                    } catch (IOException e){
                        e.printStackTrace();
                    }
                    return;
                }
            };
            thread.run();
    }
    
    private static void send_candidates(final Socket socket) throws IOException {
        Thread thread = new Thread(){
                @Override
                public void run() {
                    try {
//                        OutputStreamWriter writer = new OutputStreamWriter(socket.getOutputStream(), "UTF-8"); //Escrever no socket
                        String output = "";
                        for(int i = 0;i < candidatos.size();i++){
                            output = output + (candidatos.get(i).getNome_candidato() + "\n" + candidatos.get(i).getPartido() + "\n" + Integer.toString(candidatos.get(i).getCodigo_votacao()) + "\n\n");
//                            OutputStreamWriter writer = new OutputStreamWriter(socket.getOutputStream(), "UTF-8");
//                            writer.write("Candidate name: "+ candidatos.get(i).getNome_candidato());
//                            writer.flush();  
//
//                            writer.write(candidatos.get(i).getPartido());
//                            writer.flush();
//
//                            writer.write(candidatos.get(i).getCodigo_votacao());
//                            writer.flush();
                        }
                            OutputStreamWriter writer = new OutputStreamWriter(socket.getOutputStream(), "UTF-8");
                            writer.write(output);
                            writer.flush();  
                    } catch (IOException e){
                        e.printStackTrace();
                    }
                    return;
                }
        };
        thread.run();
    }

    
    private static void count_votes(final Socket socket) throws IOException {
        Thread thread = new Thread(){
                @Override
                public void run() {
                    try {
                        String output = "";
                        int winner  = 0, max = 0;
                        for(int i = 0;i < candidatos.size();i++){
                            output = output + (candidatos.get(i).getNome_candidato() + "\n" + candidatos.get(i).getPartido() + "\n" + Integer.toString(candidatos.get(i).getCodigo_votacao()) + "\nVotos = " + candidatos.get(i).getNum_votos() + "\n\n");
                            
                            if(candidatos.get(i).getNum_votos() > max){
                                winner = i;
                                max = candidatos.get(i).getNum_votos();
                            }
                        }
                            output = output + ("Vencedor = " + candidatos.get(winner).getNome_candidato() + "\nVotos = " + candidatos.get(winner).getNum_votos() + "\n\n");
                            OutputStreamWriter writer = new OutputStreamWriter(socket.getOutputStream(), "UTF-8");
                            writer.write(output);
                            writer.flush();  
                    } catch (IOException e){
                        e.printStackTrace();
                    }
                    return;
                }
        };
        thread.run();
    }
    
    private static String getOpcao(final Socket socket) throws IOException {
    	FutureTask<String> thread = new FutureTask<String>(new Callable<String>(){
            String str;
            BufferedReader mReader;
                @Override
                public String call() throws Exception {
                    mReader = new BufferedReader(new InputStreamReader(socket.getInputStream(), "UTF-8"));
                    str = mReader.readLine();	           
                    return str;
                }
        });
    	thread.run();
    	
        try {
            return thread.get();
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }
        return null;
    }  
}
