#include <string>
using namespace std;
class Anagrama
{
	private:
		string palavra;		
		int dist_edicao;
		double semelhanca;
	public:
		string getPalavra(){return palavra;};
		void setPalavra(string n_palavra){palavra = n_palavra;};
		int getDistEdicao(){return dist_edicao;};
		void setDistEdicao(int n_dist){dist_edicao = n_dist;};
		double getSemelhanca() const {return semelhanca;};
		void setSemelhanca(double n_semelhanca){semelhanca = n_semelhanca;};
		bool operator<(const Anagrama &input) const { return (this->semelhanca > input.getSemelhanca());};
};