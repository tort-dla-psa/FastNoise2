#pragma once
#include <vector>
#include <memory>

#include <Magnum/Magnum.h>
#include <Magnum\GL\GL.h>
#include <Magnum/GL/Texture.h>
#include <Magnum\ImageView.h>

#include "FastNoise/FastNoise.h"
#include "MeshNoisePreview.h"
#include "NoiseTexture.h"

namespace Magnum
{
    class FastNoiseNodeEditor
    {
    public:
        FastNoiseNodeEditor();
        void Draw( const Matrix4& transformation, const Matrix4& projection, const Vector3& cameraPosition );
        void CheckLinks();
        void DoContextMenu();
        void DoNodes();
        void UpdateSelected();
        int AddNode( ImVec2 startPos, const FastNoise::Metadata* metadata );

    private:
        struct Node
        {
            using Ptr = std::unique_ptr<Node>;

            Node( FastNoiseNodeEditor&, const FastNoise::Metadata* );
            void GeneratePreview();
            std::shared_ptr<FastNoise::Generator> GetGenerator( std::unordered_set<int>& dependancies, std::vector<std::unique_ptr<FastNoise::NodeData>>& nodeDatas );

            FastNoiseNodeEditor& editor;

            int id;
            const FastNoise::Metadata* metadata;
            std::string serialised;

            std::vector<int*> memberLinks;
            std::vector<int> memberNodes;
            std::vector<std::pair<int, float>> memberHybrids;
            std::vector<FastNoise::Metadata::MemberVariable::ValueUnion> memberValues;

            static const int NoiseSize = 224;
            GL::Texture2D noiseTexture;
        };

        std::shared_ptr<FastNoise::Generator> GenerateSelectedPreview();
        void ChangeSelectedNode( int newId );

        std::unordered_map<int, Node::Ptr> mNodes;
        int mCurrentNodeId = 0;
        bool mDroppedLink = false;
        int mDroppedLinkNodeId = 0;

        MeshNoisePreview mMeshNoisePreview;
        NoiseTexture mNoiseTexture;

        int mSelectedNode = 0;
        float mNodeFrequency = 0.02f;
        int mNodeSeed = 1337;
    };
}